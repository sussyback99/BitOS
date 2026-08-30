#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200

#define COLOR_BLACK        0
#define COLOR_BLUE         1
#define COLOR_WHITE        15
#define COLOR_LIGHT_BLUE   9
#define COLOR_LIGHT_GRAY   7
#define COLOR_DARK_GRAY    8

int cursor_x = 10;
int cursor_y = 25;
int input_x = 10;
int input_y = 170;
char input_buffer[256];
int input_pos = 0;
int console_line = 0;
volatile unsigned char last_scancode = 0;

inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void draw_pixel(int x, int y, unsigned char color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        volatile unsigned char* VGA = (volatile unsigned char*)0xA0000;
        VGA[y * SCREEN_WIDTH + x] = color;
    }
}

void draw_rectangle(int x, int y, int width, int height, unsigned char color) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            draw_pixel(x + j, y + i, color);
        }
    }
}

void draw_char(char c, int x, int y, unsigned char color) {
    if (c == 'A') {
        draw_rectangle(x, y + 2, 1, 5, color);
        draw_rectangle(x + 4, y + 2, 1, 5, color);
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
    }
    else if (c == 'B') {
        draw_rectangle(x, y, 2, 7, color);
        draw_rectangle(x, y, 4, 1, color);
        draw_rectangle(x, y + 3, 4, 1, color);
        draw_rectangle(x, y + 6, 4, 1, color);
        draw_pixel(x + 4, y + 1, color);
        draw_pixel(x + 4, y + 2, color);
        draw_pixel(x + 4, y + 4, color);
        draw_pixel(x + 4, y + 5, color);
    }
    else if (c == 'C') {
        draw_rectangle(x, y + 1, 1, 5, color);
        draw_rectangle(x + 1, y, 4, 1, color);
        draw_rectangle(x + 1, y + 6, 4, 1, color);
    }
    else if (c == 'D') {
        draw_rectangle(x, y, 2, 7, color);
        draw_rectangle(x + 2, y, 2, 1, color);
        draw_rectangle(x + 2, y + 6, 2, 1, color);
        draw_rectangle(x + 4, y + 1, 1, 5, color);
    }
    else if (c == 'E') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y, 4, 1, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
        draw_rectangle(x + 1, y + 6, 4, 1, color);
    }
    else if (c == 'F') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y, 4, 1, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
    }
    else if (c == 'G') {
        draw_rectangle(x, y + 1, 1, 5, color);
        draw_rectangle(x + 1, y, 4, 1, color);
        draw_rectangle(x + 1, y + 6, 4, 1, color);
        draw_rectangle(x + 3, y + 4, 2, 1, color);
        draw_rectangle(x + 4, y + 4, 1, 2, color);
    }
    else if (c == 'H') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 4, y, 1, 7, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
    }
    else if (c == 'I') {
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x + 2, y, 1, 7, color);
        draw_rectangle(x + 1, y + 6, 3, 1, color);
    }
    else if (c == 'J') {
        draw_rectangle(x + 1, y, 4, 1, color);
        draw_rectangle(x + 3, y + 1, 1, 5, color);
        draw_rectangle(x, y + 5, 3, 1, color);
        draw_pixel(x, y + 4, color);
    }
    else if (c == 'K') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y + 3, 4, 1, color);
        draw_pixel(x + 1, y + 2, color);
        draw_pixel(x + 2, y + 1, color);
        draw_pixel(x + 3, y, color);
        draw_pixel(x + 1, y + 4, color);
        draw_pixel(x + 2, y + 5, color);
        draw_pixel(x + 3, y + 6, color);
    }
    else if (c == 'L') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y + 6, 4, 1, color);
    }
    else if (c == 'M') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 4, y, 1, 7, color);
        draw_pixel(x + 1, y + 1, color);
        draw_pixel(x + 2, y + 2, color);
        draw_pixel(x + 3, y + 1, color);
    }
    else if (c == 'N') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 4, y, 1, 7, color);
        draw_pixel(x + 1, y + 1, color);
        draw_pixel(x + 2, y + 2, color);
        draw_pixel(x + 3, y + 3, color);
        draw_pixel(x + 2, y + 4, color);
        draw_pixel(x + 1, y + 5, color);
    }
    else if (c == 'O') {
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x, y + 1, 1, 5, color);
        draw_rectangle(x + 4, y + 1, 1, 5, color);
        draw_rectangle(x + 1, y + 6, 3, 1, color);
    }
    else if (c == 'P') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
        draw_rectangle(x + 4, y + 1, 1, 2, color);
    }
    else if (c == 'Q') {
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x, y + 1, 1, 5, color);
        draw_rectangle(x + 4, y + 1, 1, 5, color);
        draw_rectangle(x + 1, y + 6, 3, 1, color);
        draw_pixel(x + 3, y + 5, color);
        draw_pixel(x + 4, y + 6, color);
    }
    else if (c == 'R') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
        draw_rectangle(x + 4, y + 1, 1, 2, color);
        draw_pixel(x + 1, y + 4, color);
        draw_pixel(x + 2, y + 5, color);
        draw_pixel(x + 3, y + 6, color);
    }
    else if (c == 'S') {
        draw_rectangle(x + 1, y, 3, 1, color);
        draw_rectangle(x, y + 1, 1, 2, color);
        draw_rectangle(x + 1, y + 3, 3, 1, color);
        draw_rectangle(x + 4, y + 4, 1, 2, color);
        draw_rectangle(x + 1, y + 6, 3, 1, color);
    }
    else if (c == 'T') {
        draw_rectangle(x, y, 5, 1, color);
        draw_rectangle(x + 2, y, 1, 7, color);
    }
    else if (c == 'U') {
        draw_rectangle(x, y + 1, 1, 5, color);
        draw_rectangle(x + 4, y + 1, 1, 5, color);
        draw_rectangle(x + 1, y + 6, 3, 1, color);
    }
    else if (c == 'V') {
        draw_rectangle(x, y + 1, 1, 4, color);
        draw_rectangle(x + 4, y + 1, 1, 4, color);
        draw_rectangle(x + 1, y + 5, 3, 1, color);
        draw_pixel(x + 1, y + 4, color);
        draw_pixel(x + 3, y + 4, color);
    }
    else if (c == 'W') {
        draw_rectangle(x, y, 1, 7, color);
        draw_rectangle(x + 4, y, 1, 7, color);
        draw_pixel(x + 1, y + 4, color);
        draw_pixel(x + 2, y + 5, color);
        draw_pixel(x + 3, y + 4, color);
        draw_pixel(x + 2, y + 6, color);
    }
    else if (c == 'X') {
        draw_pixel(x, y, color);
        draw_pixel(x + 1, y + 1, color);
        draw_pixel(x + 2, y + 2, color);
        draw_pixel(x + 3, y + 3, color);
        draw_pixel(x + 1, y + 5, color);
        draw_pixel(x, y + 6, color);
        draw_pixel(x + 4, y + 1, color);
        draw_pixel(x + 3, y + 2, color);
        draw_pixel(x + 2, y + 4, color);
        draw_pixel(x + 3, y + 5, color);
        draw_pixel(x + 4, y + 6, color);
    }
    else if (c == 'Y') {
        draw_pixel(x, y, color);
        draw_pixel(x + 1, y + 1, color);
        draw_pixel(x + 2, y + 2, color);
        draw_pixel(x + 3, y + 1, color);
        draw_pixel(x + 4, y, color);
        draw_rectangle(x + 2, y + 3, 1, 4, color);
    }
    else if (c == 'Z') {
        draw_rectangle(x, y, 5, 1, color);
        draw_rectangle(x, y + 6, 5, 1, color);
        draw_pixel(x + 1, y + 1, color);
        draw_pixel(x + 2, y + 2, color);
        draw_pixel(x + 3, y + 3, color);
        draw_pixel(x + 2, y + 4, color);
        draw_pixel(x + 1, y + 5, color);
    }
    else if (c == ' ') {
    }
}

void draw_string(const char* str, int x, int y, unsigned char color) {
    int pos = x;
    for (int i = 0; str[i] != '\0'; i++) {
        draw_char(str[i], pos, y, color);
        pos += 6;
    }
}

void draw_interface() {
    draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BLACK);
    draw_rectangle(0, 0, SCREEN_WIDTH, 12, COLOR_BLUE);
    draw_char('B', 6, 3, COLOR_WHITE);
    draw_char('i', 12, 3, COLOR_WHITE);
    draw_char('t', 18, 3, COLOR_WHITE);
    draw_char('O', 24, 3, COLOR_WHITE);
    draw_char('S', 30, 3, COLOR_WHITE);
    draw_rectangle(0, 12, SCREEN_WIDTH, 1, COLOR_DARK_GRAY);
    draw_rectangle(0, 13, SCREEN_WIDTH, SCREEN_HEIGHT - 13, COLOR_LIGHT_GRAY);
    draw_rectangle(2, 14, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 16, COLOR_WHITE);
    draw_rectangle(2, 160, SCREEN_WIDTH - 4, 1, COLOR_DARK_GRAY);
    draw_rectangle(2, 161, SCREEN_WIDTH - 4, 1, COLOR_LIGHT_GRAY);
    draw_string("BitOS Console v1.0", 10, 25, COLOR_BLUE);
    draw_string("Type 'stdout.write \"text\"' to print", 10, 35, COLOR_BLACK);
    draw_string("> ", 10, 170, COLOR_BLUE);
}

void print_to_console(const char* text) {
    int y = 45 + console_line * 10;
    if (y > 155) {
        draw_rectangle(2, 14, SCREEN_WIDTH - 4, 145, COLOR_WHITE);
        console_line = 0;
        y = 45;
    }
    draw_string(text, 10, y, COLOR_BLACK);
    console_line++;
}

void execute_command(const char* cmd) {
    if (cmd[0] == 's' && cmd[1] == 't' && cmd[2] == 'd' && cmd[3] == 'o' && cmd[4] == 'u' && cmd[5] == 't' && cmd[6] == '.' && cmd[7] == 'w' && cmd[8] == 'r' && cmd[9] == 'i' && cmd[10] == 't' && cmd[11] == 'e' && cmd[12] == ' ') {
        int start = 13;
        if (cmd[start] == '"') {
            start++;
            int end = start;
            while (cmd[end] != '"' && cmd[end] != '\0') end++;
            if (cmd[end] == '"') {
                char text[256];
                int len = end - start;
                for (int i = 0; i < len; i++) text[i] = cmd[start + i];
                text[len] = '\0';
                print_to_console(text);
            }
        }
    }
}

void clear_input_area() {
    draw_rectangle(10 + 6, 170, 300, 10, COLOR_WHITE);
    draw_string("> ", 10, 170, COLOR_BLUE);
}

void process_key(unsigned char scancode) {
    static int shift_pressed = 0;
    
    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        shift_pressed = 0;
        return;
    }
    
    char key = 0;
    if (scancode == 0x1E) key = 'A';
    else if (scancode == 0x30) key = 'B';
    else if (scancode == 0x2E) key = 'C';
    else if (scancode == 0x20) key = 'D';
    else if (scancode == 0x12) key = 'E';
    else if (scancode == 0x21) key = 'F';
    else if (scancode == 0x22) key = 'G';
    else if (scancode == 0x23) key = 'H';
    else if (scancode == 0x17) key = 'I';
    else if (scancode == 0x24) key = 'J';
    else if (scancode == 0x25) key = 'K';
    else if (scancode == 0x26) key = 'L';
    else if (scancode == 0x32) key = 'M';
    else if (scancode == 0x31) key = 'N';
    else if (scancode == 0x18) key = 'O';
    else if (scancode == 0x19) key = 'P';
    else if (scancode == 0x10) key = 'Q';
    else if (scancode == 0x13) key = 'R';
    else if (scancode == 0x1F) key = 'S';
    else if (scancode == 0x14) key = 'T';
    else if (scancode == 0x16) key = 'U';
    else if (scancode == 0x2F) key = 'V';
    else if (scancode == 0x11) key = 'W';
    else if (scancode == 0x2D) key = 'X';
    else if (scancode == 0x15) key = 'Y';
    else if (scancode == 0x2C) key = 'Z';
    else if (scancode == 0x39) key = ' ';
    
    if (key && input_pos < 255) {
        input_buffer[input_pos++] = key;
        input_buffer[input_pos] = '\0';
        draw_char(key, cursor_x, 170, COLOR_BLACK);
        cursor_x += 6;
        if (cursor_x > 300) {
            cursor_x = 10;
        }
    }
    
    if (scancode == 0x1C) {
        input_buffer[input_pos] = '\0';
        print_to_console(input_buffer);
        execute_command(input_buffer);
        input_pos = 0;
        cursor_x = 10 + 6;
        clear_input_area();
    }
    
    if (scancode == 0x0E) {
        if (input_pos > 0) {
            input_pos--;
            cursor_x -= 6;
            draw_rectangle(cursor_x, 170, 6, 10, COLOR_WHITE);
            input_buffer[input_pos] = '\0';
        }
    }
}

extern "C" void keyboard_handler() {
    unsigned char scancode = inb(0x60);
    last_scancode = scancode;
}

extern "C" void kernel_main() {
    draw_interface();
    
    while (1) {
        asm volatile ("hlt");
        
        if (last_scancode) {
            process_key(last_scancode);
            last_scancode = 0;
        }
    }
}