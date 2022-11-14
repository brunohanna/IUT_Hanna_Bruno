
/* 
 * File:   Toolbox.h
 * Author: TP-EO-1
 *
 * Created on 9 septembre 2022, 16:30
 */

#ifndef TOOLBOX_H
#define	TOOLBOX_H

#ifdef	__cplusplus
extern "C" {
#endif

#define PI 3.141592653589793

float Abs(float value);
float Max(float value, float value2);
float Min(float value, float value2);
float LimitToInterval(float value, float lowLimit, float highLimit);
float RadianToDegree(float value);
float DegreeToRadian(float value);


#ifdef	__cplusplus
}
#endif

#endif	/* TOOLBOX_H */
