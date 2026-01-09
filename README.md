# TriliumRunner

A KRunner plugin that searches your local Trilium Notes database and opens matching notes directly in the Trilium Desktop app.

## Features

- Fast search through note titles and content
- Opens notes directly in Trilium Desktop
- Minimum 5-character query for better results
- Shows note preview in search results (Title only in KRunner shortcut. Title + Description in Application Launcher)

## Requirements

- KDE Plasma 6
- Trilium Desktop app installed with local database
- KDE Frameworks 6 (KF6Runner, KF6I18n, KF6Config)
- Qt 6.5+
- SQLite3

## Installation

### From Source
```bash
git clone https://github.com/pojomi/TriliumRunner.git
cd TriliumRunner
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```

Restart KRunner:
```bash
kquitapp6 krunner && krunner
```

## Usage

1. Open KRunner (default: `Alt+Space` or `Alt+F2`)
2. Type at least 5 characters to search your Trilium notes
3. Click on a result to open it in Trilium Desktop

**Note:** Trilium Desktop must not be running when you click a search result. If Trilium is already open, clicking will not navigate to the note (though it causes no harm).

## Author

pojomi
