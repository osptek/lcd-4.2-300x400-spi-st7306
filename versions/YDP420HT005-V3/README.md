<p align="left"><img alt="OSPTEK" src="./images/logo.png" width="200" /></p>

<h1 align="center">OSPTEK 4.2″ LCD 300×400（ST7306 · SPI）</h1>

<p align="center"><b>三色全反射 LCD · SPI · ST7306</b></p>

<p align="center"><a href="./README_EN.md">English</a> | 简体中文 · <a href="../../README.md">规格族索引</a></p>

<p align="center">
  <img alt="Size: 4.2 inch" src="https://img.shields.io/badge/Size-4.2%22-3498DB?style=flat-square" />
  <img alt="Resolution: 300x400" src="https://img.shields.io/badge/Resolution-300%C3%97400-8E44AD?style=flat-square" />
  <img alt="Interface: SPI" src="https://img.shields.io/badge/Interface-SPI-27AE60?style=flat-square" />
  <img alt="Driver: ST7306" src="https://img.shields.io/badge/Driver-ST7306-E7352C?style=flat-square" />
</p>

<p align="center"><img alt="OSPTEK 4.2 寸 300×400 三色全反射 LCD SPI 模组（ST7306）宣传图" src="./images/product.png" width="640" /></p>

## 目录

- [产品简介](#产品简介)
- [规格参数](#规格参数)
- [示例工程](#示例工程)
- [仓库结构](#仓库结构)
- [相关资料](#相关资料)
- [购买链接](#购买链接)
- [技术支持](#技术支持)

---

## 产品简介

OSPTEK **4.2 寸 300×400 三色全反射 LCD** 是一款 **SPI** 接口黑白红显示模组，显示驱动为 **ST7306**，触摸驱动为 **FT3269**。适合低功耗仪表、标签与户外可读场景。

规格标识（仓库名）：`lcd-4.2-300x400-spi-st7306`

当前模组版本：**YDP420HT005-V3**。外形与 FPC 定义以 [`docs/YDP420HT005-V3_外形图.pdf`](./docs/YDP420HT005-V3_%E5%A4%96%E5%BD%A2%E5%9B%BE.pdf) 为准。

## 规格参数

| 项目 | 规格 |
| ---- | ---- |
| 尺寸 | 4.2 英寸 |
| 类型 | 全反射 LCD（黑白红） |
| 分辨率 | 300×400 |
| 接口 | SPI |
| 驱动 IC | ST7306 |
| 触摸驱动 | FT3269 |

> 完整外形尺寸、FPC 定义、供电与时序以产品规格书 / 驱动手册为准。

## 示例工程

| 说明 | 路径 |
| ---- | ---- |
| ESP32-S3 · ST7306 SPI bring-up（可选 FT3269 触摸画点） | [`examples/esp32s3-4.2-tft-300x400-spi-st7306-bringup/`](./examples/esp32s3-4.2-tft-300x400-spi-st7306-bringup/) |

## 仓库结构

```text
lcd-4.2-300x400-spi-st7306/             # 仓库根（导航见 ../../README.md）
└── versions/
    └── YDP420HT005-V3/                  # 本料号完整资料
        ├── README.md
        ├── README_EN.md
        ├── images/
        ├── docs/
        └── examples/
```

## 相关资料

### 本产品资料

| 资料 | 链接 |
| ---- | ---- |
| 外形图（YDP420HT005-V3） | [`docs/YDP420HT005-V3_外形图.pdf`](./docs/YDP420HT005-V3_%E5%A4%96%E5%BD%A2%E5%9B%BE.pdf) |
| 驱动 IC 数据手册（ST7306） | [`docs/ST_7306_V0_1_c30c3541a3.pdf`](./docs/ST_7306_V0_1_c30c3541a3.pdf) |
| 2.9 / 4.2″ TP 二合一转接板原理图 | [`docs/SCH_2.9&4.2TP二合一转接板.pdf`](./docs/SCH_2.9%264.2TP%E4%BA%8C%E5%90%88%E4%B8%80%E8%BD%AC%E6%8E%A5%E6%9D%BF.pdf) |

### 示例工程

- [ESP32-S3 ST7306 SPI bring-up](./examples/esp32s3-4.2-tft-300x400-spi-st7306-bringup/) |

## 购买链接

<p align="center">
  <a href="https://shop110742373.taobao.com/"><img alt="淘宝官方店铺" src="https://img.shields.io/badge/淘宝-官方店铺-FF6A00?style=for-the-badge" /></a>
  &nbsp;&nbsp;
  <a href="https://www.aliexpress.com/store/1105701619"><img alt="速卖通官方店铺" src="https://img.shields.io/badge/速卖通-官方店铺-E62E04?style=for-the-badge&logo=aliexpress&logoColor=white" /></a>
</p>

**国内（淘宝）**

- 店铺：[鱼鹰光电工厂店](https://shop110742373.taobao.com/)

**海外（AliExpress）**

- 店铺：[OSPTEK Official Store](https://www.aliexpress.com/store/1105701619)

## 技术支持

- 技术支持 / 产品咨询：<luyu@osptek.com>
- QQ 技术交流群：**985881096**
- 公司官网：<https://osptek.com/>
- 有任何问题，都可以在本仓库 Issues 中提问

---

<p align="center"><sub>© 2026 OSPTEK 鱼鹰光电 · 本仓库资料采用 CC BY 4.0 许可</sub></p>
