//
//  NSDictionary+safeObject.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/14.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "NSDictionary+safeObject.h"

@implementation NSMutableDictionary (safeObject)

- (void)sh_setObject:(id)aObject forKey:(id<NSCopying>)akey
{
    if (aObject) {
        [self setObject:aObject forKey:akey];
    }
}

@end
