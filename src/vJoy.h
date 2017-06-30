#pragma once

#include <public.h>
#include <vjoyinterface.h>

class vJoy {
public:
    vJoy(UINT rid) :
            m_rid(rid) {
        AcquireVJD(m_rid);
    }

    virtual ~vJoy() {
        RelinquishVJD(m_rid);
    }

    //Sets an axis to some percentage of full value (0.5 is middle of the axis)
    inline void setXAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_X); }
    inline void setYAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_Y); }
    inline void setZAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_Z); }
    inline void setRXAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_RX); }
    inline void setRYAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_RY); }
    inline void setRZAxis(float percent) const { SetAxis(percent * 32000, m_rid, HID_USAGE_RZ); }

    //Sets a button pressed or not
    inline void setBtn1(bool isDown) const { SetBtn(isDown, m_rid, 1); }
    inline void setBtn2(bool isDown) const { SetBtn(isDown, m_rid, 2); }
    inline void setBtn3(bool isDown) const { SetBtn(isDown, m_rid, 3); }
    inline void setBtn4(bool isDown) const { SetBtn(isDown, m_rid, 4); }
    inline void setBtn5(bool isDown) const { SetBtn(isDown, m_rid, 5); }
    inline void setBtn6(bool isDown) const { SetBtn(isDown, m_rid, 6); }
    inline void setBtn7(bool isDown) const { SetBtn(isDown, m_rid, 7); }
    inline void setBtn8(bool isDown) const { SetBtn(isDown, m_rid, 8); }

    //Set all default inputs (axes to middle and buttons up)
    void setDefaults() {
        SetAxis(16000, m_rid, HID_USAGE_X);
        SetAxis(16000, m_rid, HID_USAGE_Y);
        SetAxis(16000, m_rid, HID_USAGE_Z);
        SetAxis(16000, m_rid, HID_USAGE_RX);
        SetAxis(16000, m_rid, HID_USAGE_RY);
        SetAxis(16000, m_rid, HID_USAGE_RZ);
        SetBtn(false, m_rid, 1);
        SetBtn(false, m_rid, 2);
        SetBtn(false, m_rid, 3);
        SetBtn(false, m_rid, 4);
        SetBtn(false, m_rid, 5);
        SetBtn(false, m_rid, 6);
        SetBtn(false, m_rid, 7);
        SetBtn(false, m_rid, 8);
    }

private:
    UINT m_rid;
};