# ⌨️ Silent Thief – C++ Keylogger

**Standalone C++ Keylogger** – designed for silent information capture, password harvesting, and identity impersonation.

---

## 🎯 **Role**
Information Stealing Expert – silently captures every keystroke and predicts user behavior.

---

## ⚡ **Capabilities**

| Ability | Description |
|---------|-------------|
| **Keystroke Capture** | Logs every key press (including special keys like Enter, Tab, Backspace) to a hidden file `/tmp/.sys_kernel_log`. |
| **Password Master** | Parses captured logs for password-related commands (`passwd`, `login`) and stores them for later use. |
| **Command Prediction** | Detects sensitive commands (`sudo`, `ssh`, `mysql`) and predicts potential privilege escalation or lateral movement. |
| **Identity Theft** | Simulates identity theft by altering environment variables and collecting user-specific credentials from logs. |
| **Silent Logging** | Runs as a daemon, masquerades as `[kworker/0:1]`, and automatically rotates logs to avoid detection. |

---

## ⚠️ **Disclaimer**

> **For educational and authorized testing only.**  
> Unauthorised use is illegal. The author assumes no responsibility for misuse.

---

## 📦 **Requirements**

- **Compiler:** `g++` (C++11 or later)
- **X11 libraries:** `libx11-dev`, `libxext-dev`
- **Build tools:** `make`, `binutils`

### Install X11 libraries (on Linux):
```bash
sudo apt install libx11-dev libxext-dev   # Debian/Ubuntu
sudo yum install libX11-devel             # RHEL/CentOS
