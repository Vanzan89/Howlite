#include "HowlitePCH.h"
#include "Buffer.h"

namespace Howlite {

	HAttribute::HAttribute(EAttributeType AttributeType, size_t Offset) :
		mOffset{ Offset }, mAttributeType{ AttributeType }
	{

	}
	
	size_t HAttribute::SizeOfType(EAttributeType AttributeType) noexcept
	{
		switch(AttributeType)
		{
			case EAttributeType::Position2D:
			{
				return sizeof(HAttributeMap<EAttributeType::Position2D>::SystemType);
			}
			case EAttributeType::Position3D:
			{
				return sizeof(HAttributeMap<EAttributeType::Position3D>::SystemType);
			}
			case EAttributeType::Normal3D:
			{
				return sizeof(HAttributeMap<EAttributeType::Normal3D>::SystemType);
			}
			case EAttributeType::UV2D:
			{
				return sizeof(HAttributeMap<EAttributeType::UV2D>::SystemType);
			}
			case EAttributeType::Color3D:
			{
				return sizeof(HAttributeMap<EAttributeType::Color3D>::SystemType);
			}
			case EAttributeType::Color4D:
			{
				return sizeof(HAttributeMap<EAttributeType::Color4D>::SystemType);
			}
		}

		return 0u;
	}
	
	size_t HAttribute::GetSize() const noexcept
	{
		return SizeOfType(mAttributeType);
	}
	
	size_t HAttribute::GetOffset() const noexcept
	{
		return mOffset;
	}
	
	size_t HAttribute::GetOffsetAfter() const noexcept
	{
		return mOffset + GetSize();
	}
	
	EAttributeType HAttribute::GetType() const noexcept
	{
		return mAttributeType;
	}
	
	D3D11_INPUT_ELEMENT_DESC HAttribute::GetDescription() const noexcept
	{
		switch (mAttributeType)
		{
			case EAttributeType::Position2D:
			{
				return CreateDescription<EAttributeType::Position2D>(mOffset);
			}
			case EAttributeType::Position3D:
			{
				return CreateDescription<EAttributeType::Position3D>(mOffset);
			}
			case EAttributeType::Normal3D:
			{
				return CreateDescription<EAttributeType::Normal3D>(mOffset);
			}
			case EAttributeType::UV2D:
			{
				return CreateDescription<EAttributeType::UV2D>(mOffset);
			}
			case EAttributeType::Color3D:
			{
				return CreateDescription<EAttributeType::Color3D>(mOffset);
			}
			case EAttributeType::Color4D:
			{
				return CreateDescription<EAttributeType::Color4D>(mOffset);
			}
		}

		return {};
	}
	
	const std::string HAttribute::GetCode() const noexcept
	{
		switch (mAttributeType)
		{
			case EAttributeType::Position2D:
			{
				return HAttributeMap<EAttributeType::Position2D>::Code;
			}
			case EAttributeType::Position3D:
			{
				return HAttributeMap<EAttributeType::Position3D>::Code;
			}
			case EAttributeType::Normal3D:
			{
				return HAttributeMap<EAttributeType::Normal3D>::Code;
			}
			case EAttributeType::UV2D:
			{
				return HAttributeMap<EAttributeType::UV2D>::Code;
			}
			case EAttributeType::Color3D:
			{
				return HAttributeMap<EAttributeType::Color3D>::Code;
			}
			case EAttributeType::Color4D:
			{
				return HAttributeMap<EAttributeType::Color4D>::Code;
			}
		}

		return {};
	}

	HLayout::HLayout(const std::initializer_list<EAttributeType>& Types)
	{
		for (EAttributeType type : Types)
		{
			mAttributes.emplace_back(type, GetSize());
		}
	}

	const HAttribute& HLayout::GetAttributeByIndex(size_t Index) const
	{
		return mAttributes[Index];
	}

	bool HLayout::Contains(EAttributeType AttributeType) const noexcept
	{
		for (const HAttribute& attribute : mAttributes)
		{
			if (attribute.GetType() == AttributeType)
			{
				return true;
			}
		}

		return false;
	}

	size_t HLayout::GetSize() const noexcept
	{
		return mAttributes.size() == 0u ? 0u : mAttributes.back().GetOffsetAfter();
	}
	
	size_t HLayout::GetAttributeCount() const noexcept
	{
		return mAttributes.size();
	}
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> HLayout::GetInputLayoutDescription() const noexcept
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
		
		layout.reserve(GetAttributeCount());
		
		for(const auto& attribute : mAttributes)
		{
			layout.push_back(attribute.GetDescription());
		}

		return layout;
	}
	
	std::string HLayout::GetCode() const noexcept
	{
		std::string code;

		for(const auto& attribute : mAttributes)
		{
			code += attribute.GetCode();
		}

		return code;
	}

	HElement::HElement(uint8_t* Data, const HLayout& Layout) :
		mData{ Data }, mLayout{ Layout }
	{
		H_ASSERT(mData != nullptr, "'mData == nullptr'");
	}

	HBuffer::HBuffer(const HLayout& Layout, size_t Size) :
		mLayout{ Layout }
	{
		Resize(Size);
	}

	void HBuffer::Resize(size_t Size)
	{
		const size_t size = GetSize();

		if(size < Size)
		{
			mData.resize(mData.size() + mLayout.GetSize() * (Size - size));
		}
	}

	const uint8_t* HBuffer::GetData() const noexcept
	{
		return mData.data();
	}

	size_t HBuffer::GetElementCount() const noexcept
	{
		return mData.size() / mLayout.GetSize();
	}

	size_t HBuffer::GetSize() const noexcept
	{
		return mData.size();
	}

	const HLayout& HBuffer::GetLayout() const noexcept
	{
		return mLayout;
	}

	HElement HBuffer::Front()
	{
		H_ASSERT(!mData.empty(), "Failed to get front element, buffer is empty.");
		return HElement{ mData.data(), mLayout };
	}

	HElement HBuffer::Back()
	{
		H_ASSERT(!mData.empty(), "Failed to get back element, buffer is empty.");
		return HElement{ mData.data() + mData.size() - mLayout.GetSize(), mLayout };
	}

	HElement HBuffer::operator[](size_t Index)
	{
		H_ASSERT(Index < GetElementCount(), "Failed to get element by index, index outside the range 'Index >= GetSize()'.");
		return HElement{ mData.data() + mLayout.GetSize() * Index, mLayout };
	}


}
