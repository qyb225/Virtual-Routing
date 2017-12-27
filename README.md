# Virtual-Routing

中心化虚拟路由系统。

仅在 Ubuntu 16.04 下测试，其它 Linux 应该也能跑，Windows 下因为 socket API 不一样无法运行。

---

**controller**: 计算路径返回给 sender_client，端口默认 8020。

```
gcc controller.c socket_utils.c graph.c -o controller
```

```
./controller
```

---

**host**: 接收 / 转发报文 (路由器功能)

```
gcc host.c socket_utils.c -o host
```

运行时开多个进程监听多个端口：

```
./host 8001
./host 8002
./host 8003
```

---

**sender_client**: 发送数据到其它 host，端口默认 8000。


```
gcc sender_client.c socket_utils.c -o sender_client
```


```
./sender_client
```


---


---

## 流程

**Step 1**: Sender_client 发送数据包到 Controller 请求路径。

数据包格式：

|字段|长度|
|---|---|
| src_ip | 32bits |
| src_port | 16bits |
| dst_ip | 32bits |
| dst_port | 16bits |

注：src 和 dst 是需要请求的路径的起点和目的地。

---

**Step 2**: Controller 利用 Dijkstra 算法得到路径，并打包返回给 Sender_client。

数据包格式：

|字段|长度|
|---|---|
| last host_ip | 32bits |
| last host_port | 16bits |
| ... | ... |
| 2nd host_ip | 32bits |
| 2nd host_port | 16bits |
| 1st host_ip | 32bits |
| 1st host_port | 16bits |

---

**Step 3**: Sender_client 得到 Controller 返回的路径数据包，打包数据包并发送给下一跳路由器 (host)

数据包格式：

|字段|长度|
|---|---|
| src_ip | 32bits |
| src_port | 16bits |
| dst_ip | 32bits |
| dst_port | 16bits |
| data_length | 32bits |
| data | n bits (depend on msg_length) |
| last host_ip | 32bits |
| last host_port | 16bits |
| ... | ... |
| 2nd host_ip | 32bits |
| 2nd host_port | 16bits |

注：数据包的最后 48 bits 信息是下一跳 host 的地址，因此在发送给下一跳 host 时， 需要先截掉最后 48 bits。

---

**Step 4**: Host 接到数据包，比较自己的 ip 和 port 是否与 dst ip 和 dst prot 相同，如果相同则数据成功到达目标主机，通过数据包拿到数据段；如果不同则截取最后 48 bits 得到下一跳 ip 和 port，然后转发即可。

