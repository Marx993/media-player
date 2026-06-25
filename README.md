# Media Player

A feature-rich media player application built using the VLC library, providing a robust and flexible solution for audio and video playback.

## About

This project is a desktop media player that leverages the powerful VLC (VideoLAN) library to support a wide range of multimedia formats. It's built primarily in C and C++ with Makefile automation for easy compilation and installation.

## Features

- Support for multiple audio and video formats via VLC backend
- Cross-platform compatibility
- Efficient and lightweight design
- Simple and intuitive user interface

## Requirements

Before installing, ensure you have the following dependencies installed:

- **VLC Library (libvlc)**
  ```bash
  # Ubuntu/Debian
  sudo apt-get install libvlc-dev vlc

  # Fedora/RHEL
  sudo dnf install vlc-devel vlc

  # macOS (with Homebrew)
  brew install vlc
  ```

- **Build Tools**
  - GCC/Clang compiler
  - Make
  - QMake (for Qt-based components)

## Installation

### From Source

1. **Clone the repository**
   ```bash
   git clone https://github.com/Marx993/media-player.git
   cd media-player
   ```

2. **Install dependencies** (see Requirements section above)

3. **Build the project**
   ```bash
   make
   ```

4. **Install the application**
   ```bash
   sudo make install
   ```

### Verify Installation

To verify the installation was successful:
```bash
media-player --version
```

## Usage

Run the media player:
```bash
media-player
```

Or if not in your PATH:
```bash
./media-player
```

## Project Structure

The project is composed of:
- **Makefile** (69.4%) - Build automation
- **C** (23%) - Core functionality
- **C++** (7.4%) - Additional components
- **QMake** (0.2%) - Qt build configuration

## Building from Source (Advanced)

For custom builds or development:

```bash
# Clean previous builds
make clean

# Build with debug symbols
make DEBUG=1

# Build specific components
make [component_name]
```

## Troubleshooting

### VLC Library Not Found

If you encounter VLC library errors during compilation:
```bash
# Find VLC pkg-config path
pkg-config --cflags --libs libvlc

# Update Makefile with correct paths if needed
```

### Build Errors

- Ensure all dependencies are installed
- Check that your compiler version supports C11/C++11 or later
- Run `make clean` before rebuilding

## Contributing

Contributions are welcome! Please feel free to submit issues and enhancement requests.

## License

Please see the LICENSE file for licensing information.

## Support

For issues, questions, or suggestions, please open an issue on the [GitHub repository](https://github.com/Marx993/media-player/issues).

---

**Built with VLC** - The power of open-source multimedia