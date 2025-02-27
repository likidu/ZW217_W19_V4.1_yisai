## FAQ

**Q：如何合理的配置download分区大小？**

A：download分区配置为需要升级的尺寸总和的10~50%之间，若空间非常紧张，建议设置为最大应用尺寸的20%。

**Q：可用RAM大小会对性能有哪些影响？**

A：ram空间不足时，第一，可能导致还原过程中内存分配失败，从而增加升级失败的风险；第二，不会启用二次压缩，相同的版本，制作的差分包会更大，分配给算法ram空间配置尽可能大，最小不低于3个block（最小擦除）大小。

**Q：升级过程中意外断电会有什么后果？**

A：升级过程中断电，重新上电后会继续升级。但是升级过程中断电有可能带来其他风险，因此推荐在升级在点亮充足的条件下进行，并且过程中明确的提示用户不要断电。

**Q：哪些因素会影响升级的时间？**

A：第一，版本差异大小。升级前后版本差异越小，需要更新的flash越少，擦写flash的总次数越少，耗时也就越少；第二，存储介质擦写耗时，升级相同的版本，存储介质擦写耗时越少，总的耗时越少；

**Q：在一个平台上适配的还原库，可以直接应用到其他项目上吗？**

A：主要取决于以下几个方面：1.芯片架构，2.编译工具，3.算法配置；如果这几项都一样即可可以直接应用到其他项目上。

**Q：还原错误码具体是什么含义？**

A：参考[README.md](./README.md)文件



