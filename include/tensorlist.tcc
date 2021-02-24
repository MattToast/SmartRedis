#ifndef SILC_TENSORLIST_TCC
#define SILC_TENSORLIST_TCC

template <class T>
TensorList<T>::~TensorList()
{
    this->_free_tensors();
}

template <class T>
TensorList<T>::TensorList(const TensorList& t_list)
{
    typename std::forward_list<Tensor<T>*>::const_iterator it =
        t_list._inventory.cbegin();
    typename std::forward_list<Tensor<T>*>::const_iterator it_end =
        t_list._inventory.cend();

    while(it!=it_end) {
        this->_inventory.push_front(new Tensor<T>(**it));
        it++;
    }
    this->_inventory.reverse();
}

template <class T>
TensorList<T>::TensorList(TensorList&& t_list)
{
    this->_inventory = std::move(t_list._inventory);
}

template <class T>
TensorList<T>& TensorList<T>::operator=(const TensorList<T>& t_list)
{
    if(this!=&t_list) {
        this->_free_tensors();

        typename std::forward_list<Tensor<T>*>::const_iterator it =
        t_list._inventory.cbegin();
        typename std::forward_list<Tensor<T>*>::const_iterator it_end =
        t_list._inventory.cend();

        while(it!=it_end) {
            this->_inventory.push_front(new Tensor<T>(**it));
            it++;
        }
        this->_inventory.reverse();
    }
    return *this;
}

template <class T>
TensorList<T>& TensorList<T>::operator=(TensorList<T>&& t_list)
{
    if(this!=&t_list) {
        this->_free_tensors();
        this->_inventory = std::move(t_list._inventory);
    }
    return *this;
}

template <class T>
void TensorList<T>::add_tensor(Tensor<T>* tensor)
{
    this->_inventory.push_front(tensor);
    return;
}

template <class T>
void TensorList<T>::add_tensor(TensorBase* tensor) {
    this->_inventory.push_front((Tensor<T>*)tensor);
}

template <class T>
Tensor<T>* TensorList<T>::allocate_tensor(const std::string& name,
                                    void* data,
                                    const std::vector<size_t>& dims,
                                    const TensorType type,
                                    const MemoryLayout mem_layout)
{
    this->_inventory.push_front(new Tensor<T>(name, data, dims,
                                              type, mem_layout));
    return this->_inventory.front();
}

template <typename T>
void TensorList<T>::clear() {
    this->_free_tensors();
}

template <typename T>
typename TensorList<T>::iterator TensorList<T>::begin() {
    return this->_inventory.begin();
}

template <typename T>
typename TensorList<T>::const_iterator TensorList<T>::cbegin() {
    return this->_inventory.cbegin();
}

template <typename T>
typename TensorList<T>::iterator TensorList<T>::end(){
    return this->_inventory.end();
}

template <typename T>
typename TensorList<T>::const_iterator TensorList<T>::cend(){
    return this->_inventory.cend();
}

template <typename T>
inline void TensorList<T>::_free_tensors() {
    while(!this->_inventory.empty()) {
        delete this->_inventory.front();
        this->_inventory.pop_front();
    }
}

#endif //SILC_TENSORLIST_TCC