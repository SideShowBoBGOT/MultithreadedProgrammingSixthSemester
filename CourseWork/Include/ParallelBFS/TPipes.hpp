#ifndef PARALLELBFS_TPIPES_HPP
#define PARALLELBFS_TPIPES_HPP

#include <concepts>
#include <memory>

namespace bfs {

template<std::semiregular T>
class TPipeReader;

template<std::semiregular T>
class TPipeWriter;

template<std::semiregular T>
class TPipeChannel;

template<std::semiregular T>
class TPipeReader {
	friend class TPipeChannel<T>;

	public:
	~TPipeReader();

	TPipeReader(TPipeReader&& other) noexcept;

	TPipeReader& operator=(TPipeReader&& other);

	public:
	T Read() const;

	protected:
	TPipeReader() = default;

	TPipeReader(const std::shared_ptr<std::pair<T, std::atomic_flag>>& data);

	TPipeReader(const TPipeReader&) = delete;

	TPipeReader& operator=(const TPipeReader&) = delete;

	protected:
	std::shared_ptr<std::pair<T, std::atomic_flag>> m_pData = nullptr;
};

template<std::semiregular T>
TPipeReader<T>::TPipeReader(const std::shared_ptr<std::pair<T, std::atomic_flag>>& data)
	: m_pData{data} {
}

template<std::semiregular T>
TPipeReader<T>::~TPipeReader() {
}

template<std::semiregular T>
TPipeReader<T>::TPipeReader(TPipeReader&& other) noexcept
	: m_pData{std::move(other.m_pData)} {
}

template<std::semiregular T>
TPipeReader<T>& TPipeReader<T>::operator=(TPipeReader&& other) {
	*this = std::move(other);
	return *this;
}

template<std::semiregular T>
T TPipeReader<T>::Read() const {
	m_pData->second.wait(false);
	auto inner = std::move(m_pData->first);
	m_pData->second.clear();
	m_pData->second.notify_one();
	return inner;
}

template<std::semiregular T>
class TPipeWriter {
	friend class TPipeChannel<T>;

	public:
	~TPipeWriter();

	TPipeWriter(TPipeWriter&& other) noexcept;

	TPipeWriter& operator=(TPipeWriter&& other);

	public:
	void Write(T&& value) const;

	protected:
	TPipeWriter() = default;

	TPipeWriter(const std::shared_ptr<std::pair<T, std::atomic_flag>>& data);

	TPipeWriter(const TPipeWriter&) = delete;

	TPipeWriter operator=(const TPipeWriter&) = delete;

	protected:
	std::shared_ptr<std::pair<T, std::atomic_flag>> m_pData = nullptr;
};

template<std::semiregular T>
TPipeWriter<T>::TPipeWriter(const std::shared_ptr<std::pair<T, std::atomic_flag>>& data)
	: m_pData{data} {
}

template<std::semiregular T>
TPipeWriter<T>::~TPipeWriter() {
}

template<std::semiregular T>
TPipeWriter<T>::TPipeWriter(TPipeWriter&& other) noexcept
	: m_pData{std::move(other.m_pData)} {
}

template<std::semiregular T>
TPipeWriter<T>& TPipeWriter<T>::operator=(TPipeWriter&& other) {
	*this = std::move(other);
	return *this;
}

template<std::semiregular T>
void TPipeWriter<T>::Write(T&& value) const {
	m_pData->second.wait(true);
	m_pData->first = std::move(value);
	m_pData->second.test_and_set();
	m_pData->second.notify_one();
}

template<std::semiregular T>
class TPipeChannel {
	public:
	TPipeChannel();

	public:
	TPipeWriter<T> Writer;
	TPipeReader<T> Reader;
};

template<std::semiregular T>
TPipeChannel<T>::TPipeChannel() {
	auto data = std::make_shared<std::pair<T, std::atomic_flag>>();
	Writer = TPipeWriter(data);
	Reader = TPipeReader(std::move(data));
}

}

#endif //PARALLELBFS_TPIPES_HPP