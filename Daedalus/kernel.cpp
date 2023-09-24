void print_char(char c, unsigned char forecolour, unsigned char backcolour, int x, int y) {
    int attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile char* video_memory = (volatile char*)(0xb8000 + (y * 80 + x) * 2);
    *video_memory = c;
    *(video_memory + 1) = attrib;
}

void print_str(const char* c) {
    int x = 0;
    int y = 0;

    for (const char* ptr = c; *ptr != '\0'; ptr++) {
        if (*ptr == '\n') {
            x = 0;
            y++;
            continue;
        }

        print_char(*ptr, 7, 0, x++, y);

        if (x < 80) continue;

        x = 0;
        y++;
    }
}

extern "C" void main() {
    const char* string = "Hello, World";
    print_str(string);
    return;
}