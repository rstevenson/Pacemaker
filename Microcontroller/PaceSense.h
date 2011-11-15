void sense_init(void);
void Update(unsigned long Tn, unsigned long Tms, unsigned long Tmp, unsigned long Tmpace,
        unsigned int VRP, unsigned int p_PW);
Bool SenseVRP (void);
Bool PaceVRP(unsigned int vPA);
Bool In_VRP(void);
int get_fmarker(void);
void setWaitInt(struct params par);
Bool vPace(unsigned int vPA);
void pace(unsigned int vPA, unsigned int VRP, unsigned long Tn);
