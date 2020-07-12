#pragma once

namespace Howlite {

	enum class HEAttributeType {
		Position2D,
		Position3D,
		Normal3D,
		Color3D,
		Color4D,
	};

	template<HEAttributeType> struct HAttributeMap;

	template<> struct HAttributeMap<HEAttributeType::Position2D>
	{
		using SystemType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT DXGIFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* Semantic = "Position";
		static constexpr const char* Code = "P2";
	};

	template<> struct HAttributeMap<HEAttributeType::Position3D>
	{
		using SystemType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT DXGIFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* Semantic = "Position";
		static constexpr const char* Code = "P3";
	};

	template<> struct HAttributeMap<HEAttributeType::Normal3D>
	{
		using SystemType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT DXGIFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* Semantic = "Normal";
		static constexpr const char* Code = "N3";
	};

	template<> struct HAttributeMap<HEAttributeType::Color3D>
	{
		using SystemType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT DXGIFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* Semantic = "Color";
		static constexpr const char* Code = "C3";
	};

	template<> struct HAttributeMap<HEAttributeType::Color4D>
	{
		using SystemType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT DXGIFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* Semantic = "Color";
		static constexpr const char* Code = "C4";
	};

	class HAttribute
	{
	public:
		HAttribute(HEAttributeType AttributeType, size_t Offset);
		~HAttribute() = default;

		/**
		 * Get Size Of Given Attribute Type
		 * @param AttributeType 
		 * @return 
		 */
		static size_t SizeOfType(HEAttributeType AttributeType) noexcept;

		/**
		 * Get Size
		 * @return 
		 */
		size_t GetSize() const noexcept;

		/**
		 * Get Offset Of Current Attribute
		 * @return 
		 */
		size_t GetOffset() const noexcept;

		/**
		 * Get Offset After Current Attribute
		 * @return
		 */
		size_t GetOffsetAfter() const noexcept;

		/**
		 * Get Type
		 * @return 
		 */
		HEAttributeType GetType() const noexcept;

		/**
		 * Get D3D11 Description
		 * @return 
		 */
		D3D11_INPUT_ELEMENT_DESC GetDescription() const noexcept;

		/**
		 * Get Code
		 * @return 
		 */
		const std::string GetCode() const noexcept;

	private:
		template<HEAttributeType Type>
		inline static constexpr D3D11_INPUT_ELEMENT_DESC CreateDescription(size_t Offset) noexcept
		{
			return { HAttributeMap<Type>::Semantic, 0, HAttributeMap<Type>::DXGIFormat, 0, (UINT)Offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		}

		size_t mOffset{ 0u };
		HEAttributeType mAttributeType;

	};

	class HLayout {
	public:
		HLayout() = default;
		~HLayout() = default;

		/**
		 * Get Attribute
		 * @return 
		 */
		template<HEAttributeType Type>
		const HAttribute& GetAttribute() const
		{
			for(const auto& attribute : mAttributes)
			{
				if(attribute.GetType() == Type)
				{
					return attribute;
				}
			}

			H_ASSERT(false, "Failed to get attribute. Attribute type mismatch.");
		}

		/**
		 * Get Size In Bytes
		 * @return 
		 */
		size_t GetSize() const noexcept;

		/**
		 * Get Number Of Attribute
		 * @return 
		 */
		size_t GetAttributeCount() const noexcept;

		/**
		 * Get Input Layout Description
		 * @return 
		 */
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputLayoutDescription() const noexcept;
		
		/**
		 * Get Code
		 * @return 
		 */
		std::string GetCode() const noexcept;

	private:
		std::vector<HAttribute> mAttributes;

	};

	class HElement {
	public:
		~HElement() = default;

		template<typename HEAttributeType Type>
		inline auto& GetAttribute()
		{
			using namespace DirectX;

			auto ptr = mData + mLayout.GetAttribute<Type>().GetOffset();

			if constexpr (Type == HEAttributeType::Position2D)
			{
				return *reinterpret_cast<XMFLOAT2*>(ptr);
			}
			else if constexpr (Type == HEAttributeType::Position3D)
			{
				return *reinterpret_cast<XMFLOAT3*>(ptr);
			}
			else if constexpr (Type == HEAttributeType::Normal3D)
			{
				return *reinterpret_cast<XMFLOAT3*>(ptr);
			}
			else if constexpr (Type == HEAttributeType::Color3D)
			{
				return *reinterpret_cast<XMFLOAT3*>(ptr);
			}
			else if constexpr (Type == HEAttributeType::Color4D)
			{
				return *reinterpret_cast<XMFLOAT4*>(ptr);
			}

			H_ASSERT(false, "Failed to get attribute. Unknown HEAttributeType.");
		}


		template<typename HEAttributeType Type, typename T>
		inline void SetAttribute(T&& Value)
		{
			using namespace DirectX;

			auto ptr = mData + mLayout.GetAttribute<Type>().GetOffset();

			if constexpr (Type == HEAttributeType::Position2D)
			{
				SetAttribute<HEAttributeType::Position2D>(ptr, std::forward<T>(Value));
			}
			else if constexpr (Type == HEAttributeType::Position3D)
			{
				SetAttribute<HEAttributeType::Position3D>(ptr, std::forward<T>(Value));
			}
			else if constexpr (Type == HEAttributeType::Normal3D)
			{
				SetAttribute<HEAttributeType::Normal3D>(ptr, std::forward<T>(Value));
			}
			else if constexpr (Type == HEAttributeType::Color3D)
			{
				SetAttribute<HEAttributeType::Color3D>(ptr, std::forward<T>(Value));
			}
			else if constexpr (Type == HEAttributeType::Color4D)
			{
				SetAttribute<HEAttributeType::Color4D>(ptr, std::forward<T>(Value));
			}

			H_ASSERT(false, "Failed to set attribute 'Unknown HEAttributeType Type'.");
		}

	private:
		friend class HBuffer;

		HElement(uint8_t* Data, const HLayout& Layout);

		template<typename DestinationType, typename SourceType>
		inline void SetAttribute(uint8_t* Attribute, SourceType&& SourceValue)
		{
			if constexpr (std::is_assignable<DestinationType, SourceType>::value)
			{
				*reinterpret_cast<DestinationType*>(Attribute) = SourceValue;
			}
			else
			{
				H_ASSERT(false, "Failed to set attribute. Parameter type mismatch 'std::is_assignable<DestinationType, SourceType>::value == false'.");
			}
		}

		uint8_t* mData{ nullptr };
		const HLayout& mLayout;

	};

	class HBuffer {
	public:
		HBuffer(const HLayout& Layout, size_t Size = 0u);
		~HBuffer() = default;

		/**
		 * Resize Buffer
		 * @param Size 
		 */
		void Resize(size_t Size);

		/**
		 * Get Data Pointer
		 * @return 
		 */
		const uint8_t* GetData() const noexcept;

		/**
		 * Get Number Of Elements
		 * @return 
		 */
		size_t GetElementCount() const noexcept;

		/**
		 * Get Size Of Buffer 
		 * @return 
		 */
		size_t GetSize() const noexcept;

		/**
		 * Get Layout
		 * @return 
		 */
		const HLayout& GetLayout() const noexcept;

		/**
		 * Get Front Element
		 * @return 
		 */
		HElement Front();

		/**
		 * Get Back Element
		 * @return 
		 */
		HElement Back();

		/**
		 * Get Element By Index
		 * @param Index 
		 * @return 
		 */
		HElement operator[](size_t Index);

	private:
		HLayout mLayout;
		std::vector<uint8_t> mData;	

	};
}


