#ifndef	GPIO_HPP
#define	GPIO_HPP

namespace manager {
	class Gpio {
	public:
		enum class State: int {
			LOW,
			HIGH
		};
		enum class Direction: int {
			IN,
			OUT
		};
		virtual ~Gpio() noexcept = default;
		virtual Direction direction() const = 0;
		virtual Gpio *clone() const = 0;
		template <class T>

		static T& cast(Gpio& gpio);

		template <class T>
		static const T& cast(const Gpio& gpio);
	};

	template <class T>
	inline T& Gpio::cast(Gpio& gpio) {
		return dynamic_cast<T&>(gpio);
	}

	template <class T>
	inline const T& Gpio::cast(const Gpio& gpio) {
		return dynamic_cast<const T&>(gpio);
	}
}
#endif // GPIO_HPP