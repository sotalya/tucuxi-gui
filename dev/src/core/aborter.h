//@@license@@

#ifndef ABORTER_H
#define ABORTER_H

namespace ezechiel {
namespace math {


/**
 * @brief The ProcessingAborter class
 * This class is passed to methods that require a huge processing time.
 * It only supply a method to test if the processing should be aborted or not.
 * If it returns true, the processing should be aborted and all resources freed.
 * If it returns true a second call would return false, so be carful with that.
 */
class ProcessingAborter
{
public:
    virtual bool shouldAbort() { return false;}
};

}
}

#endif // ABORTER_H
