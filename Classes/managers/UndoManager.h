/**
 * @file UndoManager.h
 * @brief 撤销管理器，负责存储和获取撤销记录。
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "models/UndoRecord.h"
#include <vector>

class UndoManager {
public:
    UndoManager();

    /**
     * @brief 压入一条记录
     */
    void pushRecord(const UndoRecord& record);

    /**
     * @brief 判断是否有可撤销的记录
     */
    bool canUndo() const;

    /**
     * @brief 弹出最近的一条记录（移除并返回）
     */
    UndoRecord popRecord();

    /**
     * @brief 清空所有记录
     */
    void clear();

private:
    std::vector<UndoRecord> _records;   // 记录栈
};

#endif // __UNDO_MANAGER_H__