# meta-modbas-udooneo
Yocto meta layer for [Udoo Neo](http://www.udoo.org/udoo-neo/)

*meta-modbas-udooneo* is an extension to the meta layer *meta-udoo*.

## Features
* Linux [RT preempt patch](https://rt.wiki.kernel.org/index.php/Main_Page)
* CAN bus support (FlexCAN) on Cortex-A9 of Udoo Neo 

## Installation
1. Follow the lines of [https://github.com/graugans/fsl-community-bsp-platform](https://github.com/graugans/fsl-community-bsp-platform)
2. Add
    BBLAYERS += " ${BSPDIR}/sources/meta-modbas-udooneo "
   to conf/bblayer.conf
3. bitbake core-image-base
