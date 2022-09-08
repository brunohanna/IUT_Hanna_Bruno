#ifndef TOOLBOX_H
#define TOOLBOX_H
typedef struct ToolboxStateBITS {
union {

struct {
unsigned char taskEnCours;
float vitesseGaucheConsigne;
float vitesseGaucheCommandeCourante;
float vitesseDroiteConsigne;
float vitesseDroiteCommandeCourante;}
;}
;}TOOLBOX_STATE_BITS;

extern volatile TOOLBOX_STATE_BITS ToolboxState;
#endif /* ROBOT_H */