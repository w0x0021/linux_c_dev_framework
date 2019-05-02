/*************************************************************************
    > File Name: filter.c
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年06月14日 星期三 10时32分21秒
 ************************************************************************/

/*
 * @Name                    - 冒泡排序
 * @parame      values      - 采样样本
 * @Parame      n           - 样本个数
 *
 * @return      void
 */
void bubble_sort(float *values, int n)
{
    int i,j;
    float temp;

    for (j=0; j<(n-1); j++)  
    {  
        for (i=0; i<n-j-1; i++)  
        {  
            if (values[i] > values[i+1]){  
                temp = values[i];  
                values[i] = values[i + 1];  
                values[i+1] = temp;  
            }  
        }  
    }

    return;
}

/*
 * @Name                    - 滑动窗口
 * @parame      values      - 采样样本
 * @parame      new_v       - 新值
 * @parame      n           - 数组元素个数
 *
 * @return      void
 */
void slide_sort(float *values, float new_v, int n)
{
    int i;
    for(i=0; i<n-1; i++)
    {
        values[i] = values[i+1];
    }
    values[i] = new_v;
}

/*
 * @Name                    - 滑动窗口取中值
 * @parame      values      - 采样样本
 * @parame      new_v       - 新值
 * @parame      n           - 数组元素个数
 *
 * @return      float       - 中间值
 */
float median_slide_filter(float *values, float new_v, int n)
{
    float re;
    slide_sort(values, new_v, n);

    if ((n & 1) > 0){  
        // 数组有奇数个元素，返回中间一个元素  
        re = values[(n+1)/2];  
    }else{  
        // 数组有偶数个元素，返回中间偏右的值
        re = values[n/2+1];  
    }
  
    return re;
}

/*
 * @Name 		 		- 中值滤波
 * @parame 	values 		- 需要过滤的采样样本
 * @Parame 	n 			- 样本个数
 *
 * @return 	float		- 过滤结果
 */
float median_filter(float *values, int n)  
{  
    float temp; 
    bubble_sort(values, n);

    //temp = (n&1>0) ? values[(n+1)/2] : ((values[n/2] + values[n/2+1]) / 2);

    // 计算中值  
    if ((n & 1) > 0){  
        // 数组有奇数个元素，返回中间一个元素  
        temp = values[(n+1)/2];  
    }else{  
        // 数组有偶数个元素，返回中间两个元素平均值  
        temp = (values[n/2] + values[n/2+1]) / 2;  
    }
  
    return temp;
}