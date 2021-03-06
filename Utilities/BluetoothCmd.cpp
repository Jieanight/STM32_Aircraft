#include "BluetoothCmd.h"
#include <stdint.h>

BluetoothCmd::BluetoothCmd() {
  func1_ptr_ = 0x00;
}

// 给动作执行接口 (函数指针) 赋值
/* 放弃这个构造器了，喵了个咪，直接成员赋值算了(不管特么的封装性了（╯‵□′）╯︵┴─┴) */
/*BluetoothCmd::BluetoothCmd(void(&ptr)(uint8_t)) { // 传引用
  func1_ptr_ = &ptr; // 再传地址
}*/

// 直接调用这个, 解释并执行
void BluetoothCmd::Execute(uint8_t* cmd_string) {
  switch(this->CheckString(cmd_string)){
    case(1):{
      (*func1_ptr_)(cmd_value_); // 执行第一个功能
      break;
    }
    case(2):{
      (*func2_ptr_)(cmd_value_); // 执行第二个功能
      break;
    }
    case(3):{
      (*func3_ptr_)(cmd_value_); // 执行第三个功能
      break;
    }
    default:{
      // do nothing
      break;
    }
  }
}

// 检查命令, 返回相应的解释结果
uint8_t BluetoothCmd::CheckString(uint8_t* cmd_string) {
  cmd_begin_[0] = cmd_string[0];
  cmd_begin_[1] = cmd_string[1];
  cmd_group_sel_ = cmd_string[2];
  cmd_func_sel_ = cmd_string[3];
  cmd_value_ = cmd_string[4];
  cmd_checksum_ = cmd_string[5];
  uint8_t checksum = Checksum(cmd_string,5);
  if(checksum!=cmd_checksum_) return 0; // 校验和有没有出错
  else {
    if((cmd_begin_[0]==0xA5)&&(cmd_begin_[1]==0x5A)){
      if(cmd_group_sel_==0x05) {// 检查组选
        switch(cmd_func_sel_){
          case(0xF1):{ return 1; }
          case(0xF2):{ return 2; }
          case(0xF3):{ return 3; }
        }
      }
      else return 0;
    } // 检查帧头两个字节
    else return 0; // 头不正确
  }
  return 0;
}

// n, checksum的位数, 应该传入5
uint8_t BluetoothCmd::Checksum(uint8_t* str, uint8_t n) {
  uint8_t checksum = 0;
  for (int i=0;i<n;i++) {
    checksum += str[i];
  }
  checksum += 1; // 最后加了1
  return checksum;
}
