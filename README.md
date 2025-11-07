# Password Strength Estimator (C++/Drogon + HTML/JS)

A tiny full‑stack app that serves a static web UI and a JSON API to estimate password strength.  
Backend is written in C++ using the **Drogon** web framework; frontend is plain **HTML/CSS/JS**.

## Features
- 🔐 POST `/api/estimate` accepts `{ "password": "..." }` and returns score, verdict, entropy bits, and feedback.
- 📄 Serves static frontend from `public/` (index.html, app.js, style.css).
- 👁️ Show/Hide password toggle in the UI.
- ⚙️ Clean CMake build; runs locally with a single binary.

## How it works (high level)
- **Drogon app startup**: `src/main.cpp` loads `config.json`, sets the document root to `./public`, and starts the HTTP server on port 8080.
- **Static files**: `/` and paths like `/index.html`, `/app.js`, `/style.css` are served directly from `public/` by Drogon.
- **API route**: `PasswordController::estimate` handles `POST /api/estimate`. It reads JSON from the request, calls `estimate_strength(pw)` from `src/utils/Strength.cc`, and responds with JSON.
- **Strength logic**: `estimate_strength` performs simple checks—length, character diversity (lower/upper/digit/symbol), naive sequence/repeat detection, and a rough entropy estimate. It maps these to a 0–4 score and returns feedback tips.
- **Frontend**: `public/app.js` sends the password to `/api/estimate` and renders the result. A small toggle button flips the input type between `password` and `text` to show/hide the value.

## Technologies
- **C++17**
- **[Drogon](https://github.com/drogonframework/drogon)** web framework
- **CMake** (build system)
- **HTML/CSS/JavaScript** frontend
- **LLDB/clang** on macOS (via Xcode Command Line Tools)

## Project structure
```
Password_est/
├─ CMakeLists.txt
├─ config.json
├─ public/
│  ├─ index.html
│  ├─ app.js
│  └─ style.css
├─ src/
│  ├─ main.cpp
│  ├─ controllers/
│  │  ├─ PasswordController.h
│  │  └─ PasswordController.cc
│  └─ utils/
│     ├─ Strength.h
│     └─ Strength.cc
└─ .vscode/
   ├─ settings.json
   ├─ tasks.json
   └─ launch.json
```

## Build & Run (macOS)
1) Install dependencies (once):
```bash
brew install drogon cmake ninja
xcode-select --install   # if you haven't already
```

2) Configure & build:
```bash
mkdir -p build && cd build
cmake -G Ninja ..
cmake --build .
```

3) Run from the project root (so `config.json` and `public/` are visible):
```bash
cd ..
./build/password_est
```

4) Open in a browser:
```
http://localhost:8080
```

> If you prefer running inside `build/`, copy `config.json` and `public/` into `build/` (or have CMake do it), or set `cwd` to the project root in `.vscode/launch.json`.

## API
**POST** `/api/estimate`  
Request:
```json
{ "password": "CorrectHorseBatteryStaple!" }
```
Response:
```json
{
  "score": 4,
  "verdict": "strong",
  "entropy_bits": 118.5,
  "feedback": ["... tips ..."]
}
```

## Config
`config.json` (example):
```json
{
  "listeners": [{ "address": "0.0.0.0", "port": 8080 }],
  "app": {
    "threads_num": 0,
    "document_root": "./public"
  },
  "static": {
    "enable": true,
    "index": "index.html",
    "gzip": true
  }
}
```

## Notes & Next steps
- This is a **heuristic** estimator, not a substitute for using a password manager and strong unique passwords.
- Ideas to extend:
  - Integrate a larger dictionary list or zxcvbn‑style patterns.
  - Add Have I Been Pwned API checks (k‑anonymity) to flag known breaches.
  - Add per‑policy scoring (min length, required classes, etc.).
  - Unit tests (e.g., Catch2/GoogleTest) and a GitHub Actions CI workflow.
