# 0. 快速说明
本工程是鸿鹄模组量产测试端上的声学部分测试示例，可用于量产过程的PCBA测试和整机测试

依赖于esp-idf v3.3版本，下载地址：
`https://docs.espressif.com/projects/esp-idf/en/v3.3.1/get-started/index.html`

推荐gitee下载方法：

1、git clone https://gitee.com/EspressifSystems/esp-gitee-tools.git 

2、git clone -b v3.3.1 https://gitee.com/EspressifSystems/esp-idf.git 

3、进入esp-gitee-tools ./submodule-update.sh ../esp-idf/

注意环境变量要指向这个版本的idf.

# 1. 测试方法
请结合上位机测试程序使用。
测试固件与上位机程序间通过串口通信，串口参数为：
  波特率：921600
  位宽：8bit
  校验：无
  停止位：1bit
串口协议如下：
上位机发起测试用例： prod_test testcase_name <params>，如设置设备sn： prod_test set_sn 11223344
测试固件响应：prod_test testcase_name success/fail <params>，其中success表示命令成功，fail表示命令失败；如设置设备sn响应： prod_test set_sn success 11223344
测试音频数据上传：一致性/气密性/非线性的测试方法是设备播放一段音频，通过mic录制并返回给上位机程序，音频帧前40字节为帧头，格式为：prod_test testcase_name record 512 [frame_idx]，其中frame_idx为每个帧的序号，从0开始，注意不要定义testcase_name使得帧头超过40字节，帧头后紧跟着二进制的音频pcm数据，以回车换行结束
测试音频数据结束上传：prod_test testcase_name record_end，表示音频结束帧

测试流程：
上位机程序进入测试页面后等待设备串口返回同步命令：prod_test task success [device_sn]，当收到这个命令，并判断device_sn合法后将会自动启动测试，依次按照测试用例的执行顺序开始发起串口命令
上位机也支持通过测试页面的SN输入框输入sn来发起测试，当输入sn合法，并通过串口成功写入到测试固件后，测试固件会返回prod_test task success [device_sn]，上位机程序会进入自动测试流程

测试结果：
测试结果会写入到上位机程序目录下的log文件夹，以device_sn为前缀

# 2. 测试固件修改
客户需要根据不同的硬件来修改测试固件，请参考make menuconfig的Hardware Board Version选项，设置硬件板型，如客户的硬件不在这个列表支持范围，请自行修改，主要包括：
> dsp固件和sdk文件：`main_app/componets/bds_light`
> PA驱动文件：`main_app/components/TAS5805M`，如果使用不同的PA，需要替换成对应的PA驱动
> 相关GPIO配置：`main_app/components/codec/include/userconfig.h`
