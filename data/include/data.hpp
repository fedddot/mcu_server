#ifndef	DATA_HPP
#define	DATA_HPP

namespace server {
	class Data {
	public:
		enum class Type : int {
			INT,
			STR,
			OBJECT,
			ARRAY
		};
		virtual ~Data() noexcept = 0;
		virtual Type type() const = 0;
		virtual Data *clone() const = 0;

		template <class T>
		static T& cast(Data& data);

		template <class T>
		static const T& cast(const Data& data);
	};

	inline Data::~Data() noexcept {

	}

	template <class T>
	inline T& Data::cast(Data& data) {
		return dynamic_cast<T&>(data);
	}

	template <class T>
	inline const T& Data::cast(const Data& data) {
		return dynamic_cast<const T&>(data);
	}
}
#endif // DATA_HPP