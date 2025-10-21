# 📜 CPPSerial Changelog

All notable changes to this project will be documented in this file.  
This format follows the principles of [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)  
and adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [v1.0.0] — 2025-10-21  
### 🎉 First Stable Release

#### 🧩 Added
- Modern **C++17 interface** for serial communication on Linux  
- Support for **custom baud rates**, including non-standard values  
- Configuration of **data bits, parity, and stop bits**  
- Exception-based **error handling** for improved safety  
- Clean **header-only design** for easy inclusion in embedded systems  
- **Doxygen documentation** and a concise usage example in `README.md`

#### 🐛 Fixed
- Removed redundant and unsafe `termios` usage  
- Fixed initialization issues under certain device configurations  
- Improved internal validation and descriptive error messages

#### ⚙️ Improved
- Refactored low-level communication layer for better performance  
- Enhanced naming conventions and code consistency  
- Improved readability and adherence to C++ best practices

#### 🧾 Documentation
- Added comments and Doxygen-style headers to all public classes  
- Provided getting started guide and API overview

#### 🧩 Compatibility
- ✅ **Linux only**
- 🧰 Tested on **Ubuntu 24.04**
- 🧱 Compilers: GCC 11+, Clang 14+

#### ⚖️ License
- Distributed under **GNU General Public License v3.0 (GPLv3)**  
  See [LICENSE](./LICENSE) for details.

---

## 🪄 Template for Future Releases

Use this section as a guide for documenting future versions:

```markdown
## [vX.Y.Z] — YYYY-MM-DD
### 🚀 Added
- New feature X
- Extended support for Y

### 🐛 Fixed
- Fixed issue with Z

### ⚙️ Improved
- Performance optimizations in serial read/write
- Simplified configuration handling

### 🧾 Documentation
- Updated usage examples and API docs
