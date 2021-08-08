# tetris
双人版俄罗斯方块

也可以选择经典的单人模式。事实证明双人模式很考验默契...

其他功能：设置速度、暂停、保存、加载。AI接管已经鸽了，交给有缘人来写吧。

![img](http://m.qpic.cn/psc?/V53Gw3Z23Lsxju1phNtw3RGZAi4YQ3dp/45NBuzDIW489QBoVep5mcYXV*p7rL9M8u6wrP3Qd3FnyTLovK3j*ejJp4y3jgMdke.ABO7xBjiPU*l.jA*gUmK03gKWuEqPk5PSNpH.rxBE!/b&bo=9QKwAgAAAAABF3U!&rf=viewer_4)

***

## 参考思路

背景数据结构：二维数组。以0代表空闲，1代表已被占用

方块类基本属性是锚点位置、id（确定形状，即4个小块与锚点的相对位置）。由这两个属性可以计算出它在背景中占用的绝对位置（4个坐标），这是另一个属性。 类的方法有：更新绝对位置、旋转（由一个id更新为另一个id）、左移、右移、下落、构造随机块等。

方块的下落、左右移动和旋转：查询背景数组，调用对应方法后4个绝对坐标都为空闲就可以更新块的状态，否则拒绝。 特别地，无法继续向下落时，把块的4个绝对位置坐标标记为占用，析构这个块，并构造一个新的随机块。

障碍的消除：有块析构时，从底向上按行查看背景数组是否全为占用。若是，删除行，更新数组。

存档：直接在txt中打印背景数组、当前块和下一个块的id和锚点、速度等用户设置。

## 下载链接
[game.rar]https://github.com/lichengchen/tetris/releases/download/1.0/game.rar

解压可玩
