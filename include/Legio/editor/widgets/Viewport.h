#include <Legio/editor/widgets/IWidget.h>

namespace LG 
{
    class Viewport : public IWidget 
    {
    public:
        virtual void Render() override;
    };
}