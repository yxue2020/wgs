/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_wgs.h"

/* If you declare any globals in php_wgs.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(wgs)
*/

/* True global resources - no need for thread safety here */
static char bits[5] = {16, 8, 4, 2, 1};
static char base32_map[32] = "0123456789bcdefghjkmnpqrstuvwxyz";

static double rad(double d) { return d * PI / 180.0; }

/**
 * 计算两个坐标的球面距离
 * wgs_distance(double lat1, double lng1, double lat2, double lng2);
 * 
 * @lat1    dobule  坐标A纬度
 * @lng1    double  坐标A经度
 * @lat2    dobule  坐标B纬度
 * @lng2    double  坐标B经度
 * return:  成功返回AB之间的球面距离, 错误返回NULL
 */
PHP_FUNCTION(wgs_distance)
{
    double lat1, lat2, lng1, lng2;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dddd", &lat1, &lng1, &lat2, &lng2) == FAILURE) {
        return;
    }

    if (lat1 < -90.0 || lat1 > 90.0 || lat2 < -90.0 || lat2 > 90.0) {
        return;
    }

    if (lng1 < -180.0 || lng1 > 180.0 || lng2 < -180.0 || lng2 > 180.0) {
        return;
    }

    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double a = radLat1 - radLat2;
    double b = rad(lng1) - rad(lng2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));

    s = s * EARTH_RADIUS * 1000;
    RETURN_DOUBLE(s);
}

/**
 * 使用GeoHash对坐标编码
 * geohash(double lat, double lng, [int precise])
 * 
 * @lat1        dobule  纬度
 * @lng1        double  经度
 * @precise     dobule  精准度, 默认12
 * return: 成功返回GeoHash编码, 失败返回NULL
 */
PHP_FUNCTION(wgs_geohash)
{
    int precise = 12;
    double lat, lng;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "dd|l", &lat, &lng, &precise) == FAILURE) {
        return;
    }

    if (lat < -90.0 || lat > 90.0) {
        return;
    }

    if (lng < -180.0 || lng > 180.0) {
        return;
    }

    double mid;
    double lat_min = -90.0, lat_max = 90.0;
    double lng_min = -180.0, lng_max = 180.0;

    char *s = emalloc(sizeof(char) * (precise+1));
    if (s == NULL) {
        RETURN_NULL();
    }

    unsigned int ch = 0;
    unsigned int bit = 0;

    int i = 0;
    int is_even = 1;
    while (i < precise) {

        if (is_even) {      // 纬度
            mid = (lng_min + lng_max) / 2;
            if (lng > mid) {
                ch |= bits[bit];
                lng_min = mid;
            } else {
                lng_max = mid;
            }
        } else {            // 经度
            mid = (lat_min + lat_max) / 2;
            if (lat > mid) {
                ch |= bits[bit];
                lat_min = mid;
            } else {
                lat_max = mid;
            }
        }

        is_even = !is_even;

        // 集齐5颗龙珠, 召唤神龙
        if (bit < 4) {
            bit++;
        } else {
            s[i] = base32_map[ch];
            ch = 0;
            bit = 0;
            i++;
        }
    }

    s[i] = '\0';

    RETURN_STRINGL(s, precise, 0);
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(wgs)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(wgs)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(wgs)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(wgs)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wgs)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wgs support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ wgs_functions[]
 *
 * Every user visible function must have an entry in wgs_functions[].
 */
const zend_function_entry wgs_functions[] = {
	PHP_FE(wgs_geohash,	NULL)		/* GeoHash编码. */
    PHP_FE(wgs_distance, NULL)      /* 球面距离计算. */
	PHP_FE_END	/* Must be the last line in wgs_functions[] */
};
/* }}} */

/* {{{ wgs_module_entry
 */
zend_module_entry wgs_module_entry = {
	STANDARD_MODULE_HEADER,
	"wgs",
	wgs_functions,
	PHP_MINIT(wgs),
	PHP_MSHUTDOWN(wgs),
	PHP_RINIT(wgs),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(wgs),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(wgs),
	PHP_WGS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WGS
ZEND_GET_MODULE(wgs)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
