void sense_init(void);
void Update(unsigned int Tn, unsigned int Tms, unsigned int Tmp, unsigned int Tmpace,
        unsigned int VRP, unsigned int p_PW);
Bool SenseVRP (void);
Bool PaceVRP(unsigned int vPA);
Bool In_VRP(void);
int get_fmarker(void);
void setWaitInt(struct params par);
Bool vPace(unsigned int vPA);
void pace(unsigned int vPA, unsigned int VRP, unsigned int Tn);
