/**
 * 数据库
 * @author ankoye
 */

function result (code, msg, data=null) {
  this.code = code
  this.msg = msg
  this.data = data
}

/* ------ 账户部分，存储方式，key:username value:user ------ */
function userKey(username) {
  return 'user_' + username
}
// 登录
function findUser(form) {
  const user = JSON.parse(localStorage.getItem(userKey(form.username)))
  console.log(user)
  if (user.password === undefined || user.password === null) {
    return new result(1, '用户不存在')
  } else if (user.password !== form.password) {
    return new result(1, '密码错误')
  }
  return new result(0, '登录成功', user)
}
// 注册
function saveUser(form) {
  const user = JSON.parse(localStorage.getItem(userKey(form.username)))
  if (user) {
    return new result(1, '用户已存在')
  }
  localStorage.setItem(userKey(form.username), JSON.stringify(form))
  return new result(0, '注册成功')
}
// 当前登录的用户
function saveCurrentUser(user) {
  localStorage.setItem('currentUser', JSON.stringify(user))
}
function removeCurrentUser() {
  localStorage.removeItem('currentUser')
}
function getCurrentUser() {
  const user = JSON.parse(localStorage.getItem('currentUser'))
  return new result(0, '获取当前用户', user)
}

/* ------ 购物车部分 key:cartusername, value:goods ------ */
function cartKey() {
  const { data } = getCurrentUser()
  return 'cart_' + data.username
}
// 添加购物车
function addCart(goods) {
  var { data } = getCart()
  if ( !data ) data = []
  var flag = false
  for (const item of data) if (item.id === goods.id) {
    item.num += 1; flag = true; break;
  }
  if( !flag ) { goods.num = 1; data.push(goods); }
  localStorage.setItem(cartKey(), JSON.stringify(data))
  return new result(0, '添加购物车')
}
// 设置数量
function setCart(id, num) {
  var { data } = getCart()
  for (const item of data) if (item.id == id) {
    item.num = num; break;
  }
  localStorage.setItem(cartKey(), JSON.stringify(data))
  return new result(0, '设置数量')
}
// 获取购物车
function getCart() {
  const goodsList = JSON.parse(localStorage.getItem(cartKey()))
  return new result(0, '获取购物车', goodsList)
}
function deleteCart(id) {
  const { data } = getCart()
  for (const i in data) if (data[i].id == id) {
    data.splice(i, 1); break;
  }
  localStorage.setItem(cartKey(), JSON.stringify(data))
}


/* ------ 订单部分，存储方式，key:username value:order ------ */
function orderKey() {
  const { data } = getCurrentUser()
  return 'order_' + data.username
}
// 添加订单
function addOrder(order) {
  var { data } = getOrder()
  if ( !data ) data = []
  const time = Number(order.id.substring(0, 13))
  console.log(time)
  for (const index in data) {
    const tmp = Number(data[index].id.substring(0,13))
    if (time > tmp) {
      if (index == data.length - 1) {
        data.push(order)
      }
      continue
    } else {
      data.splice(index, 0, order)
      break
    }
  }
  localStorage.setItem(orderKey(), JSON.stringify(data))
  return new result(0, '添加订单')
}
// 获取订单
function getOrder() {
  const orderList = JSON.parse(localStorage.getItem(orderKey()))
  return new result(0, '获取订单', orderList)
}
// 删除订单
function deleteOrder(id) {
  const { data } = getOrder()
  for (const i in data) if (data[i].id == id) {
    data.splice(i, 1); break;
  }
  localStorage.setItem(orderKey(), JSON.stringify(data))
  return new result(0, '删除订单')
}

/* ------ 回收站部分，存储方式，key:username value:recycle ------ */
function recycleKey() {
  const { data } = getCurrentUser()
  return 'recycle_' + data.username
}
// 添加回收站
function addRecycle(recycle) {
  var { data } = getRecycle()
  if (!data) data = []
  data.push(recycle)
  localStorage.setItem(recycleKey(), JSON.stringify(data))
  return new result(0, '添加回收站')
}
// 获取回收站
function getRecycle() {
  const recycleList = JSON.parse(localStorage.getItem(recycleKey()))
  return new result(0, '获取回收站', recycleList)
}
// 删除回收站
function deleteRecycle(id) {
  const { data } = getRecycle()
  for (const i in data) if (data[i].id == id) {
    data.splice(i, 1); break;
  }
  localStorage.setItem(recycleKey(), JSON.stringify(data))
  return new result(0, '删除回收站', data)
}