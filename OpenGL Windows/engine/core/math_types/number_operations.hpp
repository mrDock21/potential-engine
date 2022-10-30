#pragma once

namespace CustomOps {

    /**
     * Returns sign of given number
     * @param val Number to get the sign of
     * @returns 1 for positive, -1 for negative and 0 for zero
    */
    template<class T>
    int Sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    /**
     * Returns sign of given number
     * @param val Number to get the sign of
     * @returns 1 for positive (including zero), -1 for negative
    */
    template<class T>
    int SignBin(T val) {
        return (T(0) <= val) - (val < T(0));
    } 
}
