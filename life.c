/*
 * Conway's Game of Life — terminal edition (C / ANSI)
 *
 * Controls:
 *   SPACE / p   Play / Pause
 *   s           Step one generation (while paused)
 *   r           Randomize the grid
 *   c           Clear the grid
 *   1-5         Load preset pattern
 *                 1=Glider  2=Pulsar  3=Gosper Gun  4=R-pentomino  5=Acorn
 *   +/-         Speed up / slow down
 *   q / ESC     Quit
 *
 * Build:  gcc -O2 -o life life.c
 * Run:    ./life
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>

/* ── Grid ──────────────────────────────────────────────────────────────────── */

#define MAX_W 400
#define MAX_H 200

static unsigned char grid[MAX_H][MAX_W];
static unsigned char next[MAX_H][MAX_W];
static int W, H;
static int generation;
static int population;

static void grid_clear(void) {
    memset(grid, 0, sizeof(grid));
    generation = 0;
    population = 0;
}

static void grid_randomize(double density) {
    grid_clear();
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++)
            if ((double)rand() / RAND_MAX < density) {
                grid[y][x] = 1;
                population++;
            }
}

static void grid_step(void) {
    population = 0;
    memset(next, 0, sizeof(next));
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            int n = 0;
            for (int dy = -1; dy <= 1; dy++)
                for (int dx = -1; dx <= 1; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < W && ny >= 0 && ny < H)
                        n += grid[ny][nx];
                }
            if (n == 3 || (n == 2 && grid[y][x])) {
                next[y][x] = 1;
                population++;
            }
        }
    }
    memcpy(grid, next, sizeof(grid));
    generation++;
}

static void grid_set(int x, int y) {
    if (x >= 0 && x < W && y >= 0 && y < H && !grid[y][x]) {
        grid[y][x] = 1;
        population++;
    }
}

/* ── Patterns ──────────────────────────────────────────────────────────────── */

typedef struct { int dx, dy; } Offset;

static void place_pattern(const Offset *pat, int n) {
    grid_clear();
    int cx = W / 2, cy = H / 2;
    for (int i = 0; i < n; i++)
        grid_set(cx + pat[i].dx, cy + pat[i].dy);
}

/* 1: Glider */
static const Offset pat_glider[] = {
    {0,0},{1,0},{2,0},{2,-1},{1,-2}
};

/* 2: Pulsar (quarter + reflect) */
static Offset pat_pulsar[48];
static int pat_pulsar_n;

static void build_pulsar(void) {
    static const Offset q[] = {
        {2,1},{3,1},{4,1},
        {1,2},{1,3},{1,4},
        {6,2},{6,3},{6,4},
        {2,6},{3,6},{4,6},
    };
    pat_pulsar_n = 0;
    for (int i = 0; i < 12; i++) {
        int signs[4][2] = {{1,1},{-1,1},{1,-1},{-1,-1}};
        for (int s = 0; s < 4; s++) {
            int dx = q[i].dx * signs[s][0];
            int dy = q[i].dy * signs[s][1];
            /* deduplicate */
            int dup = 0;
            for (int j = 0; j < pat_pulsar_n; j++)
                if (pat_pulsar[j].dx == dx && pat_pulsar[j].dy == dy) { dup = 1; break; }
            if (!dup)
                pat_pulsar[pat_pulsar_n++] = (Offset){dx, dy};
        }
    }
}

/* 3: Gosper Glider Gun */
static const Offset pat_gun[] = {
    {0,4},{0,5},{1,4},{1,5},
    {10,4},{10,5},{10,6},{11,3},{11,7},{12,2},{12,8},{13,2},{13,8},
    {14,5},{15,3},{15,7},{16,4},{16,5},{16,6},{17,5},
    {20,2},{20,3},{20,4},{21,2},{21,3},{21,4},{22,1},{22,5},
    {24,0},{24,1},{24,5},{24,6},
    {34,2},{34,3},{35,2},{35,3},
};

/* 4: R-pentomino */
static const Offset pat_rpent[] = {
    {0,0},{1,0},{-1,0},{0,-1},{1,1}
};

/* 5: Acorn */
static const Offset pat_acorn[] = {
    {0,0},{1,0},{-3,0},{1,1},{-1,-1},{1,-1},{2,-1}
};

static void load_pattern(int id) {
    switch (id) {
        case 1: place_pattern(pat_glider, 5); break;
        case 2: place_pattern(pat_pulsar, pat_pulsar_n); break;
        case 3: place_pattern(pat_gun, 36); break;
        case 4: place_pattern(pat_rpent, 5); break;
        case 5: place_pattern(pat_acorn, 7); break;
    }
}

/* ── Terminal helpers ──────────────────────────────────────────────────────── */

static struct termios orig_termios;
static int term_raw = 0;

static void term_restore(void) {
    if (term_raw) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        term_raw = 0;
    }
    printf("\033[?25h");   /* show cursor */
    printf("\033[0m");     /* reset colors */
    printf("\033[2J\033[H"); /* clear */
    fflush(stdout);
}

static void term_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(term_restore);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    term_raw = 1;
}

static void get_term_size(int *cols, int *rows) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
    } else {
        *cols = 80;
        *rows = 24;
    }
}

static int read_key(void) {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) { /* ESC */
            unsigned char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) return 27;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) return 27;
            return -1; /* ignore arrow keys etc. */
        }
        return c;
    }
    return 0;
}

/* ── Color palette (cycles each generation) ────────────────────────────────── */

static const char *alive_colors[] = {
    "\033[42m",   /* green */
    "\033[102m",  /* bright green */
    "\033[46m",   /* cyan */
    "\033[106m",  /* bright cyan */
    "\033[43m",   /* yellow */
    "\033[103m",  /* bright yellow */
};
#define N_COLORS 6

/* ── Rendering ─────────────────────────────────────────────────────────────── */

static char render_buf[MAX_H * MAX_W * 20 + 4096];

static void render(int running, int speed_ms) {
    char *p = render_buf;

    /* cursor home */
    p += sprintf(p, "\033[H");

    /* status bar */
    const char *state = running
        ? "\033[92m> RUNNING\033[0m"
        : "\033[93m= PAUSED\033[0m";
    p += sprintf(p, " %s  Gen \033[96m%d\033[0m  Pop \033[96m%d\033[0m  "
                     "Speed \033[96m%dms\033[0m  "
                     "[\033[37mSPACE\033[0m play  \033[37ms\033[0m step  "
                     "\033[37mr\033[0m rand  \033[37m1-5\033[0m presets  "
                     "\033[37mq\033[0m quit]\033[K\n",
                 state, generation, population, speed_ms);

    const char *color = alive_colors[generation % N_COLORS];

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            if (grid[y][x]) {
                /* alive cell: colored block */
                const char *c = color;
                while (*c) *p++ = *c++;
                *p++ = ' '; *p++ = ' ';
                /* reset */
                *p++ = '\033'; *p++ = '['; *p++ = '0'; *p++ = 'm';
            } else {
                *p++ = ' '; *p++ = ' ';
            }
        }
        /* clear to end of line + newline */
        *p++ = '\033'; *p++ = '['; *p++ = 'K'; *p++ = '\n';
    }
    *p = '\0';

    (void)!write(STDOUT_FILENO, render_buf, p - render_buf);
}

/* ── Resize handling ───────────────────────────────────────────────────────── */

static volatile sig_atomic_t resize_flag = 0;

static void handle_winch(int sig) {
    (void)sig;
    resize_flag = 1;
}

static void apply_resize(void) {
    int cols, rows;
    get_term_size(&cols, &rows);
    int nw = cols / 2;
    int nh = rows - 2;
    if (nw > MAX_W) nw = MAX_W;
    if (nh > MAX_H) nh = MAX_H;
    if (nw < 10) nw = 10;
    if (nh < 5) nh = 5;

    /* Preserve cells that fit */
    unsigned char tmp[MAX_H][MAX_W];
    memcpy(tmp, grid, sizeof(grid));
    memset(grid, 0, sizeof(grid));
    population = 0;
    int mw = nw < W ? nw : W;
    int mh = nh < H ? nh : H;
    for (int y = 0; y < mh; y++)
        for (int x = 0; x < mw; x++)
            if (tmp[y][x]) { grid[y][x] = 1; population++; }

    W = nw;
    H = nh;
    printf("\033[2J");
    fflush(stdout);
}

/* ── Main ──────────────────────────────────────────────────────────────────── */

static long long now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

int main(void) {
    srand(time(NULL));
    build_pulsar();

    int cols, rows;
    get_term_size(&cols, &rows);
    W = cols / 2;
    H = rows - 2;
    if (W > MAX_W) W = MAX_W;
    if (H > MAX_H) H = MAX_H;

    grid_randomize(0.25);

    signal(SIGWINCH, handle_winch);

    term_raw_mode();
    printf("\033[?25l");  /* hide cursor */
    printf("\033[2J\033[H");
    fflush(stdout);

    int running = 1;
    int speed_ms = 100;
    long long last_step = now_ms();

    for (;;) {
        if (resize_flag) {
            resize_flag = 0;
            apply_resize();
        }

        int key = read_key();
        if (key == 'q' || key == 'Q' || key == 27 || key == 3) /* q/ESC/Ctrl-C */
            break;
        else if (key == ' ' || key == 'p' || key == 'P')
            running = !running;
        else if (key == 's' && !running)
            grid_step();
        else if (key == 'r' || key == 'R') {
            grid_randomize(0.25);
            running = 1;
        }
        else if (key == 'c' || key == 'C') {
            grid_clear();
            running = 0;
        }
        else if (key >= '1' && key <= '5') {
            load_pattern(key - '0');
            running = 1;
        }
        else if (key == '+' || key == '=')
            speed_ms = speed_ms > 20 ? speed_ms - 20 : 20;
        else if (key == '-' || key == '_')
            speed_ms = speed_ms < 1000 ? speed_ms + 20 : 1000;

        long long now = now_ms();
        if (running && now - last_step >= speed_ms) {
            grid_step();
            last_step = now;
        }

        render(running, speed_ms);
        usleep(16000); /* ~60 fps cap */
    }

    return 0;
}
