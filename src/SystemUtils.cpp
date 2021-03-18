/* main.cpp
 *
 * Copyright 2021 ztftrue
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
#include <gtk/gtk.h>
#include <string>

#include <stdlib.h> /* system, NULL, EXIT_FAILURE */
#include <iostream>
#include <stdexcept>
#include <stdio.h>

using namespace std;

class SystemUtils
{
public:
    std::string exec(const char *cmd)
    {
        char buffer[128];
        std::string result = "";
        FILE *pipe = popen(cmd, "r");
        if (!pipe)
            throw std::runtime_error("popen() failed!");
        try
        {
            while (fgets(buffer, sizeof buffer, pipe) != NULL)
            {
                result += buffer;
            }
        }
        catch (...)
        {
            pclose(pipe);
            throw std::runtime_error("run failed!");
        }
        pclose(pipe);
        return result;
    }
    bool checkIsUefi()
    {
        // https://askubuntu.com/questions/162564/how-can-i-tell-if-my-system-was-booted-as-efi-uefi-or-bios
        string result = exec("dmesg | grep \"EFI v\"");
        return !result.empty();
    }
};
