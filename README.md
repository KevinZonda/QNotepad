# QNotepad

QNotepad is a simple text editor which is indent to provide a fast and easy notpad like Windows'.

## Screenshot

| Night                        | Light                |
|------------------------------|----------------------|
| ![](docs/img/screenshot.png) | ![](docs/img/s2.png) |

## Config

Config file should be put in `<CONFIG_PATH>/config.json`.
`<CONFIG_PATH>` please refer to [`QStandardPaths::AppConfigLocation`](https://doc.qt.io/qt-6/qstandardpaths.html).

Sample config is below. To be aware, the `wordWrap` pair is valid only when `lineWrap` is `true`, or it will be ignored..

```json
{
    "font": [ "Consolas", "Courier New", "Courier", "monospace" ],
    "fontSize": 17,
    "nativeTitleBar": true,
    "lineWrap": true,
    "wordWrap": true
}
```

## Command Tool (macOS only)

Like Windows' `notepad.exe`, QNotepad provides its command tool.  
Use below command to install:

```bash
sudo cp ./notepad /usr/local/bin
sudo chmod +x /usr/local/bin/notepad
```

Then run `notepad` in terminal to open QNotepad.