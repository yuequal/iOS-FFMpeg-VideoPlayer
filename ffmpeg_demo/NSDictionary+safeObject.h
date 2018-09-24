//
//  NSDictionary+safeObject.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/14.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSMutableDictionary (safeObject)

- (void)sh_setObject:(id)aObject forKey:(id<NSCopying>)akey;

- (id)sh_objectForKey:(id<NSCopying>)aKey;

@end
