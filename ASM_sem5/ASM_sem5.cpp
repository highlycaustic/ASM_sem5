#include <iostream>

void task1_1() {
    unsigned char result;
    __asm {
        mov al, 0xAB
        mov result, al
    }
    std::cout << "DEC: " << +result << std::endl;
    std::cout << "HEX: " << "0x" << std::hex << +result << std::endl;
}

void task1_2() {
    int input;
    
    std::cout << "To AH register (8bit): ";
    std::cin >> input;
    char var_ah = input;
    
    std::cout << "To BX register (16bit): ";
    std::cin >> input;
    short var_bx = input;
    
    std::cout << "To EAX register (32bit): ";
    std::cin >> input;
    int var_edx = input;

    std::cout << "Entered values: " << +var_ah << ' ' << var_bx << ' ' << var_edx << std::endl;

    char ah_out;    // -128 : 127
    short bx_out;   // −32768 : 32767
    int edx_out;    // −2,147,483,648 : 2,147,483,647

    __asm {
        mov ah, var_ah
        mov ah_out, ah

        mov bx, var_bx
        mov bx_out, bx

        mov edx, var_edx
        mov edx_out, edx
    }
    std::cout << "Values from registers: " << +ah_out << ' ' << bx_out << ' ' << edx_out << std::endl;
}

void task1_3() {
    short input;
    int result;

    std::cout << "Enter 16bit number to ECX register: ";
    std::cin >> input;

    __asm {
        movsx ecx, [input]  // movsx - из меньшего операнда в больший с сохранением знака
        mov [result], ecx
    }
    std::cout << "From ECX: " << result << std::endl;
}

void task2() {
    int x;
    int y;
    int result;

    std::cout << "Enter x: ";
    std::cin >> x;
    std::cout << "Enter y: ";
    std::cin >> y;

    // edx:eax -> mul, ebx ecx free
    __asm {
        mov eax, [x]
        imul [x]      // x^2
        mov ebx, eax  // x^2 --> ebx

        mov eax, [y]
        imul [y]      // y^2
        mov ecx, eax  // y^2 --> ecx
        
        mov eax, 6
        push eax      // 6 --> stack
            
        mov eax, 7
        imul ebx
        imul [y]
        push eax      // 7x^2y --> stack
        
        mov eax, 2
        imul [x]
        imul ecx
        push eax      // 2xy^2 --> stack

        mov eax, 3
        imul ebx
        imul ecx      // eax = 3x^2y^2

        pop edx	      // 2xy^2 <-- stack
        sub eax, edx  // 3x^2y^2 - 2xy^2

        pop edx	      // 7x^2y <-- stack
        sub eax, edx  // 3x^2y^2 - 2xy^2 - 7x^2y

        pop edx	      // 6 <-- stack
        add eax, edx  // 3x^2y^2 - 2xy^2 - 7x^2y + 6

        mov [result], eax
    }
    std::cout << "Result by ASM: " << result << std::endl;

    int a = x;
    int b = y;
    int apow = a * a;
    int bpow = b * b;

    int res;

    res = (3 * apow * bpow) - (2 * a * bpow) - (7 * apow * b) + 6;
    std::cout << "Result by C++: " << res << std::endl;
}

void task3() {
    int x;

    std::cout << "Enter number to check if it is even or odd: ";
    std::cin >> x;

    char flag;
    __asm {
        mov eax, [x]
        test al, 1      // тестировать последний бит: 00000001
        jz if_even      // если последний бит 0 - прыгнуть в if_even
        mov[flag], 0
        jmp end_if

        if_even:
                mov [flag], 1
        end_if:
    }
    if (flag == 1) {
        std::cout << "Number is even" << std::endl;
    }
    else if (flag == 0) {
        std::cout << "Number is odd" << std::endl;
    }

}

void task4() {
    unsigned int n;

    std::cout << "Enter natural n: ";
    std::cin >> n;
    int* a_arr = new int[n];

    for (size_t i = 0; i < n; i++) {
        int temp;
        std::cout << "Enter a" << i << ": ";
        std::cin >> temp;
        a_arr[i] = temp;
    }
    
    int result;
    __asm {
        mov esi, [a_arr]        // развернуть указатель на массив -> записываем в ESI адрес 1 элемента массива
        mov ebx, [n]            // счетчик из n заносим в EBX
        sub ebx, 1              // EBX - 1 чтобы не выйти за границы массива
        mov ecx, 1              // Стартовая позиция счетчика
        mov eax, [esi]          // Первый элемент массива загоняем в EAX для умножения
        jmp begin_loop

        begin_loop:
            lea esi, [esi + 4]  // Перемещаемся с каждой итерацией вперед на 1 элемент (4 байта)
            mul [esi]           // Умножаем предыдущий результат на текущий элемент массива
            cmp ecx, ebx        // Сравниваем текущую позицию счетчика с верхней границей EBX
            jz end_loop         // Если счетчик и граница сравнялись, прыгаем в конец программы
            inc ecx             // Увеличиваем счетчик на 1
            jmp begin_loop      // Повтор цикла

        end_loop:		
            mov [result], eax
    }
    std::cout << result;
}

int main() {
    int choice;
    std::cout << "Enter task number: ";
    std::cin >> choice;
    switch (choice) {
        case 11:
            task1_1();
            break;
        case 12:
            task1_2();
            break;
        case 13:
            task1_3();
            break;
        case 2:
            task2();
            break;
        case 3:
            task3();
            break;
        case 4:
            task4();
            break;
        default:
            break;
    }
    //system("pause");
}