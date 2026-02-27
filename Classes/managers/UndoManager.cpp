/**
 * @file UndoManager.cpp
 * @brief UndoManager的实现文件，定义了UndoManager类的成员函数
 */

#include "UndoManager.h"

// 构造函数，初始化记录栈
UndoManager::UndoManager() {
}

// 压入一条记录
void UndoManager::pushRecord(const UndoRecord& record) {
    _records.push_back(record);
}

// 判断是否有可撤销的记录
bool UndoManager::canUndo() const {
    return !_records.empty();
}

// 弹出最近的一条记录（移除并返回）
UndoRecord UndoManager::popRecord() {
    if (_records.empty()) return UndoRecord();  // 返回一个默认构造的空记录
    UndoRecord last = _records.back();
    _records.pop_back();
    return last;
}

// 清空所有记录
void UndoManager::clear() {
    _records.clear();
}