# spring-arch

Now , it only use test.

[中文](./中文.md)

## TODO

- Search item for select
- Disk manage
- Memory manage
- tweak inline and static filed
- step button control
- Password manager(now the sh file can see password)

## Build and run

```sh
# root
meson setup builddir && cd builddir && meson setup --wipe && cd ../ && ninja -C builddir/ &&sudo ./builddir/src/spring-arch
# general user
meson setup builddir && cd builddir && meson setup --wipe && cd ../ && ninja -C builddir/ && ./builddir/src/spring-arch
```

## Use  

[refer to wiki](https://wiki.archlinux.org/index.php/Install_Arch_Linux_from_existing_Linux)

Prepare: least one U-disk or other store-tool, one computer that is your want to install archlinux , good network.

### Make U-disk system

you can use puppy or other liunx distribution. In there , i use [puppy FossaPup64](https://www.puppylinux.com/index.html#download).

1. Make U-disk system , the U-disk has anthoer volume for store `archlinux bootstap`(`no archlinux.iso`, like this `archlinux-bootstrap-2021.03.01-x86_64.tar.gz` ) and this software, i called it is f-volume.

    ```js
    ../
    arch/                                             01-Mar-2021 14:40         -
    archlinux-2021.03.01-x86_64.iso                   01-Mar-2021 14:42  769994752
    archlinux-2021.03.01-x86_64.iso.sig               01-Mar-2021 14:44        310
    archlinux-2021.03.01-x86_64.iso.torrent           01-Mar-2021 14:44      29596
    `archlinux-bootstrap-2021.03.01-x86_64.tar.gz     01-Mar-2021 14:43  147276017`
    archlinux-bootstrap-2021.03.01-x86_64.tar.gz.sig  01-Mar-2021 14:44        310
    md5sums.txt                                       01-Mar-2021 14:44        145
    sha1sums.txt                                      01-Mar-2021 14:44        161
    ```

2. Copy archlinux-startstap , this software and the `software` file to the U-disk.

3. Start the U-disk system.

### Install

1. Connect network  use puppy(U-disk system).

2. Install gpart-gui or other disk-utils, then use it fomat disk-volume of  your computer.

3. Comfirm  the f-volume is Ext4 or other linux filesystem.

4. Uncompress bootstrap in the f-volume.   `tar xzf <bootstrap filepath>/archlinux-bootstrap-<>.gz`

    The console output error:

    ```sh
    tar: Ignoring unknown extended header keyword 'LIBARCHIVE.xattr.    security.capability'
    tar: Ignoring unknown extended header keyword 'LIBARCHIVE.xattr.    security.capability'
    ```

    Don’t Worry About It, your file is correctly.

5. Copy the `software` file in the f-volume/archlinux-bootstrap.

6. Start this software(maybe you need make it can run) `./spring-arch`.

7. Now , you can run this software to install archlinux.
