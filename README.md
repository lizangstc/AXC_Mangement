# AXC_Mangement
Axc Manage Algorithm

AXC管理算法的目的就是找出一块合理的区域分给CRM

场景1：
先建立1个 1T2R 的UMTS小区，此时UL（后续例子都以UL为主）占用0 —— 18,18——36 区间的AXC，

再建立1个 1T2R 的UMTS小区，此时UL（后续例子都以UL为主）占用36 —— 54,54——72 区间的AXC，

如果用户删除了小区1，则区间 0 —— 18， 18 —— 36区间的AXC被释放，

如果重新建立  1T2R 的U小区，又会重新分0 —— 18， 18 —— 36区间的AXC，

如果重新建立的 2T2R的 L小区，L小区的ULAXC位宽为 192, 空闲的 0 —— 36 不够分，那么会从72开始往后分。

之后又建立GSM小区，G小区的UL位宽为5，此时占用的是 0 —— 5的区间，前面剩余的空间为 5 —— 36。
