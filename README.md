# Electronic_Engineering_1
电子科技大学英才实验学院工创1项目


# 工创总结

#### 第六组 李思进 刘美芳 简芸琦

#### 2022.5.14

## 功能

### 基本功能 害羞机器人

我是害羞机器人

有人靠近我，我会离你而去，离得越近，我就逃得越快。你来追我，我还会发声抱怨。

人类看不到后面的动静，我也一样，只有你从后面偷袭我（摸我头），我才能感受到，并说你坏坏！

如果周围一直没人，我会开始自在地转圈圈，自我陶醉暂不社恐，有人靠近时，我又会立刻停下并躲避。

### 拓展功能 遥控车

摸头遮光三秒，进入遥控车模式。我会建立WiFi,你可以在屏幕上看到WiFi的SSID和密码,连接,进入显示的地址即可控制。

## 创新点

### 构思创意

后面没有超声波，就像人一样，如果不看后面会撞墙，提醒人走路要看路，骑车开车也要眼观六路确保安全。在学校人车密集场所，安全尤为重要。

### 结构创意
PCB钻孔 利用小车车板M3间距固定。

### 模块创意

小车需要12V供电，实测功率20+W，功率较大，使用了现成的支持12V输出的充电宝进行供电，比淘宝电池更安全，容量更大，也降低了采购成本。

### 程序创意

需要检测距离，决策，控制运动，播放声音，屏幕显示，其中一些操作需要尽可能并行。

最方便的方式是使用FreeRtos提供的多任务

多任务有时会出bug，CPU报错，解决后I2C输出不稳定，会花屏。

最后理清操作和反馈逻辑，单线程执行

网页按键控制使用了
```html
<td ontouchstart='onTouchStartAndEnd("") 'ontouchend='onTouchStartAndEnd("")'></td>
```
TCP实现通信。

### 外观创意
使用快递盒纸板，绿色环保。


## 整体成本

|物品|价格|
| :-:  | :-: |
|小车结构|22.60|
|电机驱动|13|
|电源|8.9|
|超声波模组|8.76|
|屏幕|7.95|
|连接物| 5.8 |
|PCB |  0 |
|外壳| 0 |
|采购总计|67.01|
|---|---|
|试错、损坏、备用件|117|
|项目总计|184|

感谢英才科协、嘉立创提供的元件和免费PCB版。



References:

https://github.com/un0038998/WiFiCar


# To be continued......
# Report is still on the way.
