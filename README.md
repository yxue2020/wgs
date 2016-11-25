PHP Extension WGS
=================

 * 版本要求 PHP > 5.6


##Table of contents

 * [wgs_geohash](#wgs_geohash)
 * [wgs_distance](#wgs_distance)

### wgs_geohash
-----
_**Description**_: 使用GeoHash编码坐标

##### *Parameters*
*latitude*
*longitude*
	
##### *Return value*
*String* 成功返回Hash串, 失败返回`NULL`

##### *Examples*
~~~
$hash = geohash(39.962386,116.368561);
$hash = geohash(39.962386,116.368561, 8);
~~~

### wgs_distance
-----
_**Description**_: 计算2个坐标的球面距离.

##### *Parameters*
*latitude1*
*longitude1*
*latitude2*
*longitude2*

##### *Return value*
*Double* 成功返回两个坐标之间的距离, 单位M, 失败返回`NULL`

##### *Examples*
~~~
$d = wgs_distance(39.992904,116.293030, 39.992904,116.398030);
~~~
