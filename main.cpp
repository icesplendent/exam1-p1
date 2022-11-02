#include "LCD.h"
#include "mbed.h"

// table to show Morse Code in LED
int A[3] = {2, 1, 3};
int B[5] = {4, 3, 1, 1, 1};
int C[5] = {4, 3, 1, 3, 1};
int D[4] = {3, 3, 1, 1};
int E[2] = {1, 1};
int F[5] = {4, 1, 1, 3, 1};
int G[4] = {3, 3, 3, 1};
int H[5] = {4, 1, 1, 1, 1};
int I[3] = {2, 1, 1};
int J[5] = {4, 1, 3, 3, 3};
int K[4] = {3, 3, 1, 3};
int L[5] = {4, 1, 3, 1, 1};
int M[3] = {2, 3, 3};
int N[3] = {2, 3, 1};
int O[4] = {3, 3, 3, 3};
int P[5] = {4, 1, 3, 3, 1};
int Q[5] = {4, 3, 3, 1, 3};
int R[4] = {3, 1, 3, 1};
int S[4] = {3, 1, 1, 1};
int T[2] = {1, 1};
int U[4] = {3, 1, 1, 3};
int V[5] = {4, 1, 1, 1, 3};
int W[4] = {3, 1, 3, 3};
int X[5] = {4, 3, 1, 1, 3};
int Y[5] = {4, 3, 1, 3, 3};
int Z[5] = {4, 3, 3, 1, 1};
int one[6] = {5, 1, 3, 3, 3, 3};
int two[6] = {5, 1, 1, 3, 3, 3};
int three[6] = {5, 1, 1, 1, 3, 3};
int four[6] = {5, 1, 1, 1, 1, 3};
int five[6] = {5, 1, 1, 1, 1, 1};
int six[6] = {5, 3, 1, 1, 1, 1};
int seven[6] = {5, 3, 3, 1, 1, 1};
int eight[6] = {5, 3, 3, 3, 1, 1};
int nine[6] = {5, 3, 3, 3, 3, 1};
int zero[6] = {5, 3, 3, 3, 3, 3};

int* table[36] = {A,   B,     C,    D,    E,   F,     G,     H,    I,
                  J,   K,     L,    M,    N,   O,     P,     Q,    R,
                  S,   T,     U,    V,    W,   X,     Y,     Z,    one,
                  two, three, four, five, six, seven, eight, nine, zero};

// store the value read from the button
int temp[10];

// store the Morse Code
int data[19];

// define the button to press: pressed is 0, unpressed is 1
DigitalIn button(BUTTON1);

// define the light to shown
DigitalOut myled(LED1);

Thread generator;

void morse() {
    // Blink LED
    while (1) {
        // count if pass the seven units
        int count = 0;
        int start = 0;

        if (button == 0) {
            start = 1;
        }

        // printf("i am outside\n");

        if (start) {
            for (int index = 0; index < 19; index++) {
                // read the data from the button
                for (int i = 0; i < 10; i++) {
                    temp[i] = button;
                    ThisThread::sleep_for(100ms);
                }

                // initial data value
                data[index] = 0;

                for (int i = 0; i < 6; i++) {
                    if (temp[i] == 0 && temp[i + 1] == 0 && temp[i + 2] == 0 &&
                        temp[i + 3] == 0 && temp[i + 4] == 0) {
                        data[index] = 1;
                        count = 0;
                        printf("reading 1\n");
                        break;
                    }

                    if (i == 5) {
                        printf("reading 0\n");
                        count++;
                    }
                }

                if (count == 7) {
                    printf(
                        "finish reading the data, time to print out the "
                        "data!\n");
                    data[index] = -1;
                    break;
                }
            }

            // show the result
            for (int i = 0; i < 19 && data[i] != -1; i++) {
                if (data[i] == 1 && data[i + 1] == 1 && data[i + 2] == 1) {
                    myled = 1;
                    ThisThread::sleep_for(1500ms);
                    myled = 0;
                    i += 2;
                    printf("3\n");
                } else {
                    myled = data[i];
                    ThisThread::sleep_for(500ms);
                    myled = 0;
                    printf("%d\n", data[i]);
                }
            }

            printf("the result is shown!\n\n\n");
            for (int i = 0; i < 19; i++) {
                printf("%d ", data[i]);
            }
            printf("\n");
        }
    }
}

// main() runs in its own thread in the OS
int main() {
    // check that myled object is initialized and connected to a pin
    if (myled.is_connected()) {
        printf("myled is initialized and connected!\n\r");
    }

    generator.start(morse);

    // show the result via LCD;
    LCD_init();            // call the initialise function
    display_to_LCD(0x48);  // ‘H’
    display_to_LCD(0x45);  // ‘E’
    display_to_LCD(0x4C);  // ‘L’
    display_to_LCD(0x4C);  // ‘L’
    display_to_LCD(0x4F);  // ‘O’
    for (char x = 0x30; x <= 0x39; x++) {
        display_to_LCD(x);  // display numbers 0-9
    }
}
