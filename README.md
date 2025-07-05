# 🖥️ Simple C HTTP Server

A lightweight HTTP server built using raw **Winsock2** on Windows. It serves static files from a `public/` folder and supports basic MIME types like HTML, CSS, JS, images, and plain text.

---
## 🚀 Features

- ✅ Serve static files from `public/` directory

- ✅ Handles `GET` requests

- ✅ Returns correct MIME types

- ✅ Handles 404 (Not Found) gracefully

- ✅ Logs served files and requests

- ❌ No threading (single-client at a time)

- ❌ Only supports HTTP `GET` method

---
## 🛠️ Requirements

- Windows OS

- Visual Studio / GCC (MinGW) / Clang

- C Compiler with `Winsock2` support

---
## 🧱 Folder Structure

```
project-root/
├── server.c           # Your main C file (this program)
├── public/            # Place your static files here
│   ├── index.html     # Default file served at /
│   ├── style.css
│   └── script.js
```

---
## ⚙️ How to Build and run

Use any Windows C compiler. Example with `gcc` (MinGW):

```bash
gcc server.c -o server.exe -lws2_32
```

then run this.

```bash
./server.exe
```

Open a browser and visit:
📡 **[http://localhost:8080](http://localhost:8080)**

---

## 📝 How It Works

- Uses Winsock to create a TCP server on port `8080`

- Accepts connections and reads HTTP requests

- Supports only `GET` method

- Serves files from `public/` (default to `index.html` for `/`)

- Sends appropriate `Content-Type` header based on file extension

- Gracefully handles missing files with a 404 response

---

## 🌐 Supported MIME Types

| Extension | MIME Type                  |
| --------- | -------------------------- |
| `.html`   | `text/html`                |
| `.css`    | `text/css`                 |
| `.js`     | `application/javascript`   |
| `.png`    | `image/png`                |
| `.jpg`    | `image/jpeg`               |
| `.jpeg`   | `image/jpeg`               |
| `.txt`    | `text/plain`               |
| Other     | `application/octet-stream` |

---
## ⚠️ Limitations

Its a basic one so it have some limitation which are as following

- No HTTPS

- No request routing

- No security handling (e.g., path traversal)

- Only one client served at a time (no concurrency/threading)

---
## 🧠 Author Notes

This project is built purely for **learning purposes** — to understand how sockets, HTTP, and static file serving work at the lowest level in C on Windows.

Happy building! 🚀
