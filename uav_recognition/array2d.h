#ifndef ARRAY_2D_H
#define ARRAY_2D_H

#include <irrlicht.h>

template <class T> 
class array2d {
    T** data;
    irr::u32 w, h;
    
public:
    array2d() : w(0), h(0) {}
    
    array2d(int width, int height) 
        : w(width), h(height) 
    {
        if (width > 0 && height > 0) {
            data = new T*[w];
            for(int i = 0; i < int(w); ++i) 
                data[i] = new T[h];
        }
    }
    
    virtual void reset(int width, int height) {
        if(w && h) {
            for(int i = 0; i < int(w); i++) delete 
                data[i];
            delete [] data;
        }
        
        if(width > 0 && height > 0) {
            w = width;
            h = height;
        
            data = new T*[w];
            for(int i = 0; i < int(w); i++) 
                data[i] = new T[h];
        }
        else {
            w = 0;
            h = 0;
        }
    }
    
    ~array2d() {
        if(w > 0 && h > 0) {
            for(int i=0; i<int(w); i++) delete data[i];
            delete [] data;
        }
    }
    
    virtual T& operator ()(irr::u32 index1, irr::u32 index2) {
        return data[index1][index2];
    }
    
    virtual irr::u32 width() {return w;}
    
    virtual irr::u32 height() {return h;}
};

#endif /* ARRAY_2D_H */