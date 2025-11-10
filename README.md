# 🖼️ ESH Image Toolkit

**ESH Image Toolkit** is a custom-built image processing project written entirely in **C**.  
It allows you to convert standard BMP images(biBitCount=24,biCompression=0,biHeight>0) into a custom image format called **.esh**,  
and then view them using a pixel-by-pixel SDL2-based viewer.

---

## 🚀 Features

- 🧱 **Custom Image Format (.esh)** — designed from scratch with a simple header structure.
- 🧩 **Header Fields**

  ```c
  #pragma pack(push, 1)
  typedef struct
  {
      int16_t eshType;
      int32_t eshHeight;
      int32_t eshWidth;
      int32_t eshSize;
      int32_t eshOffBits;
  } ESHHEADER;
  #pragma pack(pop)

    eshType → File signature

    eshHeight → Image height (in pixels)

    eshWidth → Image width (in pixels)

    eshSize → Total file size

    eshOffBits → Pixel data offset

    Each pixel is stored in 4 bytes (R, G, B, A) format.

    ⚙️ CLI Support — Convert and view images directly from the terminal.

    🎨 SDL2 Viewer — Renders images pixel by pixel using the SDL2 graphics library.

    🛠️ Developed from Scratch — No external frameworks except SDL2.
  ```

📸 How It Works

    Take any BMP image as input.

    Convert it into .esh format using your converter tool.

    Open the .esh file using your custom ESH Viewer.

    The viewer reads the header, extracts pixel data, and renders the image pixel by pixel on screen.

🧑‍💻 Usage
1️⃣ Convert BMP → ESH

./bmp2esh input.bmp output.esh

2️⃣ View ESH Image

./esh_viewer output.esh

    Make sure SDL2 is installed on your system before running the viewer.

🧱 Build Instructions
🔧 Requirements

    GCC or any C compiler

    SDL2 development library

🏗️ Build Command

gcc bmp2esh.c -o bmp2esh
gcc esh_viewer.c -o esh_viewer -lSDL2

🧠 Future Improvements

    Add compression support

    Include metadata (author, timestamp, etc.)

    Support for alpha transparency and filters

    GUI-based converter & viewer

👨‍💻 Developer

Developed by Prashant Shaw

💡 B.Tech CSE | 2nd Year | Passionate about low-level graphics and systems programming.
🏷️ License

This project is open-source. Feel free to explore, modify, and learn from it.

---