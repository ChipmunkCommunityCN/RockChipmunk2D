//
//  ContentLayer.h
//  PhysicsTest
//
//  Created by ChildhoodAndy on 14-3-9.
//
//

#ifndef __PhysicsTest__ContentLayer__
#define __PhysicsTest__ContentLayer__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
class ContentLayer : public cocos2d::Layer, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    static Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(ContentLayer);
    
    virtual void scrollViewDidScroll(ScrollView* view) {};
    virtual void scrollViewDidZoom(ScrollView* view) {}
    virtual void tableCellHighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell);
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView *table);
};

#endif /* defined(__PhysicsTest__ContentLayer__) */
