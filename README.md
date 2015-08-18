# CC-Link

Closed Captioning library reading from a Link decoder.

Asio is included as a submodule. If running with openFrameworks, make sure you update submodules and include the asio library in your header search paths.

```bash
git submodule update --init --recursive
```

### Usage

To determine which COM port string to use, it's helpful to list serial devices
On the Mac, they use a format like "/dev/tty.usbserial-AL00APKE"
You can type `ls /dev/tty.*` in terminal to list the available COM ports on your machine.

In the future, we may provide a function to enumerate ports on OSX. All it does is look at /dev/ and try to match any files with the name tty.*.
