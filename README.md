# JPEG Recovery Tool

A simple program in C to recover JPEG files from a raw memory card image.

## 📌 What it does

This tool scans through a `.raw` file (binary dump of a memory card) and recovers all embedded JPEG images by identifying JPEG file signatures and writing them out as individual files (`000.jpg`, `001.jpg`, etc).

This was originally built as part of the CS50 course, but refactored and modularized for clarity, maintainability, and educational purposes.

## 🔧 How it works

- Reads 512-byte blocks from the memory card
- Detects the beginning of JPEG files by checking for a specific byte pattern
- Creates new files when a JPEG header is found
- Writes subsequent blocks into the current JPEG file until a new one is found

## 📘 What I learned
- How to work with binary file data in C
- How to identify and extract JPEG file signatures
- Modularizing and refactoring C code for clarity

## 🛠️ How to compile and run

```bash
gcc recover.c -o recover
./recover card.raw
