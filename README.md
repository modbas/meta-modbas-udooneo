# meta-modbas-udooneo
Yocto meta layer for [Udoo Neo](http://www.udoo.org/udoo-neo/)

**meta-modbas-udooneo** is an extension to the meta layer
[meta-udoo](https://github.com/graugans/meta-udoo)

## Features
* Linux [RT preempt patch](https://rt.wiki.kernel.org/index.php/Main_Page)
* CAN bus support (FlexCAN) on ARM-Cortex-A9 of Udoo Neo 

## Configuration and Build
1. Follow the lines of [https://github.com/graugans/fsl-community-bsp-platform](https://github.com/graugans/fsl-community-bsp-platform)
2. Add `BBLAYERS += " ${BSPDIR}/sources/meta-modbas-udooneo "`
to `build/conf/bblayers.conf`
3. Replace `build/conf/local.conf` by `build.conf/local.conf` 
4. `. ./setup_enviroment build`
5. `bitbake core-image-base`
