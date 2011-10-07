

enum FnCode
{
    p_pacingState = 0,// Size 1
    p_pacingMode = 1, //Size 1
    p_hysteresis = 2, //Size 1
    p_hysteresisInterval = 3, //Size 2
    p_lowRateInterval = 5, //Size 2
    p_vPaceAmp = 7, //Size 2
    p_vPaceWidth = 9, //Size 2 Check requirments rev 2 from Avenue to determine meaning of 10*p_vPaceWidth
    p_VRP = 11, //Size 2r
};

struct dataPackage
{
    public System.Byte SYNC;
    public System.Byte FnCode;
    public System.Byte[] data;
    public System.Byte ChkSum;
}