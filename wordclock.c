#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


const char letters[8][8] = {
    "VIJFTIEN",
    "VOOROVER",
    "HALF*ZES",
    "TWEENVEN",
    "AALFDRIE",
    "VIERVIJF",
    "TIENEGEN",
    "ACHT*UUR"
};

const uint8_t VIJF[8] =      { 0b11110000, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t TIEN[8] =      { 0b00001111, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t VIJFTIEN[8] =  { 0b11111111, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t VOOR[8] =      { 0, 0b11110000, 0, 0, 0, 0, 0, 0 };
const uint8_t OVER[8] =      { 0, 0b00001111, 0, 0, 0, 0, 0, 0 };
const uint8_t HALF[8] =      { 0, 0, 0b11110000, 0, 0, 0, 0, 0 };
const uint8_t UUR[8] =       { 0, 0, 0, 0, 0, 0, 0, 0b00000111 };

const uint8_t hours[][8] = {
    { 0, 0, 0b00000000, 0b11000000, 0b11110000, 0b00000000, 0b00000000, 0b00000000 }, // 12
    { 0, 0, 0b00000000, 0b00111000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, // 1
    { 0, 0, 0b00000000, 0b11110000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, // 2
    { 0, 0, 0b00000000, 0b00000000, 0b00001111, 0b00000000, 0b00000000, 0b00000000 }, // 3
    { 0, 0, 0b00000000, 0b00000000, 0b00000000, 0b11110000, 0b00000000, 0b00000000 }, // 4
    { 0, 0, 0b00000000, 0b00000000, 0b00000000, 0b00001111, 0b00000000, 0b00000000 }, // 5
    { 0, 0, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, // 6
    { 0, 0, 0b00000110, 0b00000111, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, // 7
    { 0, 0, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000 }, // 8
    { 0, 0, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000000 }, // 9
    { 0, 0, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b11110000, 0b00000000 }, // 10
    { 0, 0, 0b00000000, 0b00100000, 0b00110000, 0b00000000, 0b00000000, 0b00000000 }  // 11
};

void add(uint8_t matrix[8], const uint8_t letter[8])
{
    int y;
    for (y = 0; y < 8; y++) {
        matrix[y] |= letter[y];
    }
}

void render(uint8_t matrix[8])
{
    int x, y;
    for (y = 0; y < 8; y++) {
        uint8_t l = matrix[y];
        uint8_t m = 0x80;
        for (x = 0; x < 8; x++) {
            if ((l & m) != 0) {
                printf("%c", letters[y][x]);
            } else {
                printf(".");
            }
            m = m >> 1;
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <hour> <minute>\n", argv[0]);
        return -1;
    }

    int h = atoi(argv[1]) % 12;
    int m = atoi(argv[2]);

    uint8_t matrix[8];
    memset(matrix, 0, sizeof(matrix));

    const uint8_t *hour = hours[h];
    const uint8_t *next = hours[(h + 1) % 12];

    if (m < 3) {
        add(matrix, hour);
        add(matrix, UUR);
    } else if (m < 8) {
        add(matrix, VIJF);
        add(matrix, OVER);
        add(matrix, hour);
    } else if (m < 13) {
        add(matrix, TIEN);
        add(matrix, OVER);
        add(matrix, hour);
    } else if (m < 18) {
        add(matrix, VIJF);
        add(matrix, TIEN);
        add(matrix, OVER);
        add(matrix, hour);
    } else if (m < 23) {
        add(matrix, TIEN);
        add(matrix, VOOR);
        add(matrix, HALF);
        add(matrix, next);
    } else if (m < 28) {
        add(matrix, VIJF);
        add(matrix, VOOR);
        add(matrix, HALF);
        add(matrix, next);
    } else if (m < 33) {
        add(matrix, HALF);
        add(matrix, next);
    } else if (m < 38) {
        add(matrix, VIJF);
        add(matrix, OVER);
        add(matrix, HALF);
        add(matrix, next);
    } else if (m < 43) {
        add(matrix, TIEN);
        add(matrix, OVER);
        add(matrix, HALF);
        add(matrix, next);
    } else if (m < 48) {
        add(matrix, VIJF);
        add(matrix, TIEN);
        add(matrix, VOOR);
        add(matrix, next);
    } else if (m < 53) {
        add(matrix, TIEN);
        add(matrix, VOOR);
        add(matrix, next);
    } else if (m < 58) {
        add(matrix, VIJF);
        add(matrix, VOOR);
        add(matrix, next);
    } else {
        add(matrix, next);
        add(matrix, UUR);
    }

    render(matrix);

    return 0;
}
