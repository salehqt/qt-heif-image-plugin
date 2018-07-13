#ifndef QTHEIFIMAGEPLUGIN_IO_HANDLER_H_
#define QTHEIFIMAGEPLUGIN_IO_HANDLER_H_

#include <libheif/heif_cxx.h>

#include <QIODevice>
#include <QImageIOHandler>
#include <QSize>

#include <memory>
#include <vector>

namespace qtheifimageplugin {

class IOHandler : public QImageIOHandler
{
public:
    enum class Format
    {
        None,
        Heif,
        HeifSequence,
        Heic,
        HeicSequence,
    };

    explicit IOHandler();
    virtual ~IOHandler();

    IOHandler(const IOHandler& handler) = delete;
    IOHandler& operator=(const IOHandler& handler) = delete;

    bool canRead() const override;
    bool read(QImage* image) override;

    bool write(const QImage& image) override;

    int currentImageNumber() const override;
    int imageCount() const override;
    bool jumpToImage(int index) override;
    bool jumpToNextImage() override;

    QVariant option(ImageOption opt) const override;
    void setOption(ImageOption opt, const QVariant& value) override;
    bool supportsOption(ImageOption opt) const override;

    static Format canReadFrom(QIODevice& device);

private:
    /**
     * Holds data needed for both image decoding (in read()) and metadata
     * retrieval (in option()).
     */
    struct ReadState
    {
        const QByteArray fileData;  // must exist for lifetime of context
        heif::Context context{};
        std::vector<heif_item_id> idList{};
        int currentIndex{};
    };

    /**
     * Updates device and associated state upon device change.
     */
    void updateDevice();

    /**
     * Reads image data from device.
     * Throws heif::Error.
     */
    void loadContext();

    //
    // Private data
    //

    QIODevice* _device = nullptr;

    std::unique_ptr<ReadState> _readState;

    int _quality;
};

}  // namespace qtheifimageplugin

#endif  // QTHEIFIMAGEPLUGIN_IO_HANDLER_H_
