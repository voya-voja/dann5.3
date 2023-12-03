#pragma once
#include <memory>
#include <stdexcept>

#include <map>

using namespace std;

namespace dann5 {

	// The Factory template to be explicitely instantiated
	// to create a singleton factory pattern for a given 
	// combination of KEY (int) and a BASE class (MyBaseClass):
	// .... template class Factory<int, MyBaseClass>;
	//
	// KEY is a type of identifier, which identifies a sub-class
	// of the BASE-class for which an object should be created
	template <typename KEY, typename BASE>
	class Factory
	{
	public:
		// A static member, returns a singleton instance to the
		// explicitely instantiated Factory for a specific
		// combination of KEY and BASE
		static Factory<KEY, BASE>& Instance() 
		{ 
			return gFactory; 
		};

		// A singleton factory method which creates an object of
		// a sub-class of the BASE-class for a given key
		// Returns a shated BASE-class pointer of the created object 
		shared_ptr<BASE> create(KEY key)
		{
			return mCreators[key]->create();
		};

		// An abstraction provides a template for Creator class that 
		// creates sub-class instances of C_BASE-class for a given
		// C_KEY type 
		// The Creator template should be explicitely instantiated for
		// appropriate combination of C_KEY (int) and C_BASE types 
		// (MyBaseClass):
		// .... template class Factory<int, MyBaseClass>::Creator<int, MyBaseClass>;
		template<typename C_KEY, typename C_BASE>
		class Creator
		{
		public:
			// Factory::Creator constructor enforces specification of associated 
			// key with the specific creator.
			// Posts this Creator instance with the factory of same C_KEY
			// and C_BASE type to be used to create instances of sub-class
			// for a given key  
			Creator(C_KEY key)
				:mKey(key)
			{
				Factory<C_KEY, C_BASE>::Instance().post(this);
			};

			// Factory:: Creator destructor
			~Creator() {};

			// Override to create an instance of a sub-class of C_BASE-class
			// and return a shared pointer of C_BASE-class
			virtual shared_ptr<C_BASE> create() const = 0;

			// returns a key associated with this Creator instance
			C_KEY key() { return mKey; };

		protected:
		private:
			C_KEY mKey;	// The creator's key
		};

	protected:
	private:
		// Factory implements singleton pattern
		// default constructor is private
		Factory() {};

		// Factory destructor
		~Factory() {};

		// Posts a creator for a given Cretor pointer 
		void post(Creator<KEY, BASE>* pCreator)
		{
			KEY aKey = pCreator->key();
			if(mCreators.find(aKey) != mCreators.cend())
				throw std::logic_error("Error@Factory: try to use a same 'key' to registed a new 'creator'!");
			mCreators.emplace(aKey, pCreator);
		};
		// Factory singeton instance is a global variable
#ifdef _WINDOWS // error C2512: 'dann5::Factory<string,QuboTable>': no appropriate default constructor available
		static			Factory<KEY, BASE>		gFactory;
#else
		static inline	Factory<KEY, BASE>		gFactory;
#endif
		// A map of Factor::Creator pointers associated with their keys
		std::map<KEY, Creator<KEY, BASE> * >	mCreators;
	};

	// A default specialization of Factory::Creator abstration
	// that for a given key of type KEY creates instances using
	// a default constructor of TYPE, which are specializations
	// of BASE-class.
	// Instantiate a specific DefaultConstructor as a global variable:
	// ....DefaultCreator<int, MyBaseClass, MyBaseSpecial_1> gSpecializationCreator(1);
	// ....DefaultCreator<int, MyBaseClass, MyBaseSpecial_2> gSpecializationCreator(2);
	template<typename KEY, typename BASE, typename TYPE>
	class DefaultCreator : Factory<KEY,BASE>::template Creator<KEY, BASE>
	{
	public:
		// The costructor enforces specification of associated key
		DefaultCreator(KEY key)
			:Factory<KEY, BASE>::template Creator<KEY, BASE>(key)
		{};

		// DefaultCreator's destructor
		~DefaultCreator() {};

		// Creates an instance of TYPE using its default constructor
		// Returns a shared pointer to BASE of created instance.
		virtual shared_ptr<BASE> create() const
		{
			return(shared_ptr<BASE>(new TYPE()));
		};

	protected:
	private:
	};
};
