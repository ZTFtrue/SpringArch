
v2ray
yay -Syu onedrive-abraunegg
yay -Syu rime-luna-pinyin
pacman -Syu okular
libinput-gestures-setup restart

# mariadb

sudo systemctl start mariadb.service

sudo mysql\_install\_db --user=mysql --basedir=/usr --datadir=/var/lib/mysql

# yay install

makepkg -si

# Rust

curl https://sh.rustup.rs -sSf | sh

## Eclipse 需要

rustup component add rls

# 安装npm需要的开发包

npm install -g express @angular/cli

yay -Syu ttf-symbola ttf-twemoji-color

# btrfs 优化

```bash
# btrfs 禁用写时复制 ,在log卷中
sudo chattr +C /var/log/journal/*
# 查看用量
btrfs filesystem usage /
# 碎片整理
sudo btrfs filesystem defragment -r /
sudo btrfs filesystem defragment -r /home
```


# 开关机时间优化

```bash
systemctl mask lvm2-lvmetad.service

systemctl mask lvm2-lvmetad.socket

systemctl enable shutdown-diagnose.service

systemd-analyze

systemd-analyze blame
```


## 强制重启系统

内核启动参数添加 sysrq_always_enabled=1

```conf
GRUB_CMDLINE_LINUX_DEFAULT="quiet nouveau.blacklist=0 acpi_osi=! acpi_osi=\"Windows 2015\" mem_sleep_default=deep sysrq_always_enabled=1"
```

Reboot Even If System Utterly Broken

| 键盘快捷键             | 描述                                      |
| ---------------------- | ----------------------------------------- |
| Alt+SysRq+R+ Unraw     | 从X收回对键盘的控制                       |
| Alt+SysRq+E+ Terminate | 向所有进程发送SIGTERM信号，让它们正常终止 |
| Alt+SysRq+I+ Kill      | 向所有进程发送SIGKILL信号，强制立即终止   |
| Alt+SysRq+S+ Sync      | 将待写数据写入磁盘                        |
| Alt+SysRq+U+ Unmount   | 卸载所有硬盘然后重新按只读模式挂载        |
| Alt+SysRq+B+ Reboot    | 重启   |


## 安装AUR中的软件包

```bash
# nautilus-admin
# deepin.com.qq.office
yay -Syu google-chrome 
yay -Syu aic94xx-firmware wd719x-firmware upd72020x-fw 
yay -Syu mongodb-compass mongodb-bin gitkraken visual-studio-code-bin  libinput-gestures hfsprogs
sudo systemctl enable mongodb.service
# HP打印驱动 , 如果打印机不能使用可以尝试
yay -Syu foo2zjs-nightly

yay -Syu libsynctex
yay -Syu ttf-ms-fonts onlyoffice
```

## 设置qq分辨率

```bash

# deepin-winecfg

# deepin-wineboot

# 设置为 192
env WINEPREFIX="$HOME/.deepinwine/Deepin-TIM" winecfg

#"/opt/deepinwine/apps/Deepin-TIM/run.sh" -u %u
```

## 杀毒

```bash
sudo freshclam # update virus database
#clamscan 可用以扫描文件, 用户目录亦或是整个系统：
$ clamscan myfile
$ clamscan -r -i /home
clamscan --recursive --infected --exclude-dir='^/sys|^/dev' /
# 如果希望 clamscan 删除感染的文件，请使用 --remove 参数。
# 使用 -l path/to/file 参数可以将 clamscan 的日志写入 log 文件。
```

## gnote 同步

```bash
yay -Syu wdfs
pacman -Syu python-fuse
pacman -Syu fuse3
```

## 无法安装

```sh
yay -Syu peazip
```

## 游戏引擎

```sh
yay -Syu godot
```
