/*************************************************************************
    > File Name: filter.h
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年06月14日 星期三 10时32分21秒
 ************************************************************************/

#ifndef __FILTER_H__
#define __FILTER_H__

/*
 * @Name                 - 冒泡排序
 * @Parame   values      - 需要过滤的采样样本
 * @Parame   n           - 样本个数
 */
void bubble_sort(float *values, int n);

/*
 * @Name                    - 滑动窗口
 * @parame      values      - 采样样本
 * @parame      new_v       - 新值
 * @parame      n           - 数组元素个数
 *
 * @return      void
 */
void slide_sort(float *values, float new_v, int n);

/*
 * @Name 		 		- 中值滤波
 * @Parame	values 		- 需要过滤的采样样本
 * @Parame 	n 			- 样本个数
 *
 * @return 				- 过滤结果
 */
float median_filter(float *values, int n);

#endif