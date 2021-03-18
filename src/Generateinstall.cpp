#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <stack>
#include <list>
#include "SystemUtils.cpp"
#include "FileUtils.cpp"

using namespace std;
namespace fs = std::filesystem;

class Generateinstall
{
public:
    string generate(bool isEFI, bool isIntel,
                    string timeZones, string keyborads,
                    list<string> sourceCheck, list<string> languageCheck,
                    list<string> softwareCheck, string displayManager,
                    std::map<string, string> disks, std::map<string, string> recordDisk, string computerName, string rootPassword = "",
                    string userName = "",
                    string userPassword = "", string bootstrap = ".")
    {
        string result = "#!/bin/sh\necho 'Start install' \ncd " + bootstrap;
        for (std::list<string>::iterator it = sourceCheck.begin(); it != sourceCheck.end(); ++it)
        {
            // bootstrap no sed
            string i(*it);
            result = result +
                     "\n" +
                     "sed -i '1i\\" + i + "\\n' " + bootstrap + "/etc/pacman.d/mirrorlist";
        }
        result = result + "\n" + "cp /etc/resolv.conf etc";
        result = result + "\n" + "mount --rbind /proc proc";
        result = result + "\n" + "mount --rbind /sys sys";
        result = result + "\n" + "mount --rbind /dev dev";
        result = result + "\n" + "mount --rbind /run run";
        // result = result + "\n" + "mount --bind directory-to-livecd-or-bootstrap directory-to-livecd-or-bootstrap";
        result = result + "\n" + "chroot " + bootstrap + " /bin/bash";
        result = result + "\n" + "pacman-key --init";
        result = result + "\n" + "pacman-key --populate archlinux";
        // TODO check is it need? no free enough disk space
        // result = result + "\n" + "pacman -Sy --noconfirm  base base-devel parted";
        if (!keyborads.empty())
        {
            result = result + "\n" + "loadkeys " + keyborads;
        }
        string rootDisk = "";
        string rootMount = "";
        string bootMount = "";
        string otherMount = "";
        for (std::map<string, string>::iterator it = disks.begin(); it != disks.end(); ++it)
        {

            if (it->second == "/")
            {
                // mount -o compress=zstd /dev/nvme0n1p6 /mnt   
                // -o
                rootMount = rootMount + "\n" + "mount  /dev/" + it->first + " /mnt";
                rootDisk = it->first;
            }
            else if (it->second == "/boot")
            { // mount boot
                bootMount = bootMount + "\n" + "mkdir /mnt/boot";
                bootMount = bootMount + "\n" + "mount /dev/" + it->first + " /mnt/boot";
            }
            else
            {
                //  -o  only use in btrfs
                otherMount = otherMount + "\n" + "mount /dev/" + it->first + " /mnt" + it->second;
            }
        }
        result = result + "\n" + rootMount + "\n" + bootMount;
        result = result + "\n" + "echo -e \"\\n\" |pacstrap /mnt base base-devel  linux linux-firmware dhcpcd";
        result = result + "\n" + otherMount;
        // mount -o compress=zstd /dev/nvme0n1p7 /mnt/home
        result = result + "\n" + "genfstab -U /mnt >> /mnt/etc/fstab";
        result = result + "\n" + "arch-chroot /mnt";
        result = result + "\n" + "ln -sf " + timeZones + " /etc/localtime";
        result = result + "\n" + "hwclock --systohc";
        // result = result + "\n" + "timedatectl set-local-rtc true";
        // result = result + "\n" + "timedatectl set-ntp true";
        for (std::list<string>::iterator it = languageCheck.begin(); it != languageCheck.end(); ++it)
        {
            string i(*it);
            result = result +
                     "\n" +
                     "sed -i '1i\\" + i + "\\n' " + "/etc/locale.gen";
        }
        result = result + "\n" + "locale-gen";
        result = result + "\n" + "touch /etc/locale.conf";
        result = result + "\n" + "touch /etc/hostname";
        //   设置local
        result = result + "\n" + "sed -i '1i\\" + "LANG=en_US.UTF-8 " + "\\n' " + "/etc/locale.conf";
        result = result + "\n" + "sed -i '1i\\" + computerName + "\\n' " + "/etc/hostname";

        string hosts = "127.0.0.1	    localhost\\n127.0.1.1	" + computerName + "\\n# The following lines are desirable for IPv6 capable hosts\\n::1     ip6-localhost ip6-loopback\\n" +
                       "fe00::0 ip6-localnet\\nff00::0 ip6-mcastprefix\\nff02::1 ip6-allnodes\\nff02::2 ip6-allrouters\\n";
        result = result + "\n" + "sed -i '1i\\" + hosts + "\\n' " + "/etc/hostname";
        /**
         * When the grub boot-loader is used,
         *  the grub-mkconfig may detect devices incorrectly.
         *  This will result in Error:
         *  no such device(warning failed to connect to lvmetad，falling back to device scanning.)
         *  when * trying to boot from the stick. To solve this problem,
         *  from the host running Arch Linux, m
         * ount the newly installed partitions, arch-chroot 
         * to the new partition, then install and configure grub. 
         * The last step may require disabling lvmetad from 
         * /etc/lvm/lvm.conf 
         * by setting 
         * use_lvmetad=0  . 
         * **/
        if (isIntel)
        {
            result = result + "\n" + "echo -e \"\\n\" |pacman -S --noconfirm  intel-ucode";
        }
        result = result + "\n" + "echo -e \"\\n\" |pacman -S --noconfirm  os-prober ntfs-3g grub";
        if (isEFI)
        {
            result = result + "\n" + "echo -e \"\\n\" |pacman -S --noconfirm efibootmgr";
            // TODO check
            result = result + "\n" + "grub-install --target=x86_64-efi --efi-directory=/boot --removable --bootloader-id=ArchLinux";
        }
        else
        {
            std::map<string, string>::iterator it;
            it = recordDisk.find(rootDisk);
            if (it != recordDisk.end())
            {
                result = result + "\n" + "grub-install --target=i386-pc " + it->second;
            }
        }
        result = result + "\n" + "grub-mkconfig -o /boot/grub/grub.cfg";
        result = result + "\n" + "echo -e \"\\n\" |pacman -Syu sudo vim dialog wpa_supplicant ntfs-3g networkmanager netctl network-manager-applet xdg-user-dirs";

        for (std::list<string>::iterator it = softwareCheck.begin(); it != softwareCheck.end(); ++it)
        {
            string i(*it);
            result = result +
                     "\n" +
                     "echo -e \"\\n\" |pacman -S --noconfirm " + i;
        }
        // set root password
        result = result + "\n" + "usermod --password $(openssl passwd -1 '" + rootPassword + "') root";
        // add user
        result = result + "\n" + "sudo useradd -m -G wheel -p $(openssl passwd -1 '" + userPassword + "') " + userName;
        result = result + "\n" + "sed -i '1i\\\\n %wheel ALL=(ALL)ALL\\n' /etc/sudoers";
        result = result + "\n" + "systemctl disable netctl";
        result = result + "\n" + "systemctl enable NetworkManager";
        result = result + "\n" + "sudo systemctl enable bumblebeed.service";

        result = result + "\n" + "echo -e \"\\n\" |pacman  -S --noconfirm " + displayManager;
        result = result + "\n" + "systemctl enable " + displayManager;

        result = result + "\n" + "gpasswd -a " + userName + " input";
        // limit log size
        result = result + "\n" + "journalctl --vacuum-time=4weeks";
        result = result + "\n" + "journalctl --vacuum-size=50M";
        // generate user file
        result = result + "\n" + "xdg-user-dirs-update";
        // disable gnome file tracke https://askubuntu.com/questions/346211/tracker-store-and-tracker-miner-fs-eating-up-my-cpu-on-every-startup
        // result = result + "\n" + "echo -e \"\nHidden=true\n\" | sudo tee --append /etc/xdg/autostart/tracker-extract.desktop /etc/xdg/autostart/tracker-miner-apps.desktop /etc/xdg/autostart/tracker-miner-fs.desktop /etc/xdg/autostart/tracker-miner-user-guides.desktop /etc/xdg/autostart/tracker-store.desktop > /dev/null";
        // result = result + "\n" + "gsettings set org.freedesktop.Tracker.Miner.Files crawling-interval -2  ";
        // result = result + "\n" + "gsettings set org.freedesktop.Tracker.Miner.Files enable-monitors false";
        // result = result + "\n" + "tracker reset --hard";
        // btrfs 禁用写时复制 ,在log卷中
        // result = result + "\n" + "chattr +C /var/log/journal/*";

        // Print and network config.
        // result = result + "\n" + "systemctl enable avahi-daemon.service";
        // result = result + "\n" + "systemctl disable systemd-resolved.service";
        // result = result + "\n" + "systemctl start avahi-daemon.service";
        // result = result + "\n" + "systemctl enable org.cups.cupsd.service";
        // result = result + "\n" + "systemctl start org.cups.cupsd.service";
        // if has other system , again run it, comfirm checked other system.
        result = result + "\n" + "grub-mkconfig -o /boot/grub/grub.cfg";
        result = result + "\n" + "exit";
        result = result + "\n" + "umount -R /mnt";
        // reboot

        FileUtils file;
        file.writeFile(result, bootstrap + "/install.sh");
        SystemUtils systemUtils;
        string runCom = "chmod +x " + bootstrap + "/install.sh";
        systemUtils.exec(runCom.c_str());
        return bootstrap + "/install.sh";
    }
};
