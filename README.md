### Information
A minimalistic analog speedometer that matches HUD design.

https://github.com/kubikas3/speedo_sa/assets/37352173/a2a34287-30f8-427f-b179-8611d5eb4fbd

### Installation
1. Copy `libSpeedo.so` (if you have v2.10 then `libSpeedo64.so`) **file** to `com.rockstargames.gtasa/mods` **folder**;
2. Copy `texture` **folder** to `com.rockstargames.gtasa/files` **folder**.

### Configuration
```ini
[Display]
DialArcAngle=150.000000
DialMaxValue=250.000000
DialTickCount=25
; metric (kmph) or imperial (mph)
MeasurementSystem=metric
NitroArcMaskEndAngle=-33.000000
NitroArcMaskStartAngle=123.000000

[Layout]
OffsetX=0.000000
OffsetY=0.000000
; auto or number
PositionX=auto
; auto or number
PositionY=auto
ScaleX=2.000000
ScaleY=2.000000
; auto or number
SizeX=auto
; auto or number
SizeY=auto

[Palette]
; RGBA
ArrowColor=255 39 44 255
DialColor=0 0 0 255
NitroActiveColor=115 255 250 255
NitroLoadingColor=152 152 152 255
NumbersColor=255 255 255 255
```
*You don't have to copy it, the file is generated automatically*
