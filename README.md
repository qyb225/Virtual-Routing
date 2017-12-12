# Virtual-Routing

中心化虚拟路由系统。

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
| data_length | 16bits |
| data | n bits (depend on msg_length) |
| last host_ip | 32bits |
| last host_port | 16bits |
| ... | ... |
| 2nd host_ip | 32bits |
| 2nd host_port | 16bits |

注：数据包的最后 48 bits 信息是下一跳 host 的地址，因此在发送给下一跳 host 时， 需要先截掉最后 48 bits。

---

**Step 4**: Host 接到数据包，比较自己的 ip 和 port 是否与 dst ip 和 dst prot 相同，如果相同则数据成功到达目标主机，通过数据包拿到数据段；如果不同则截取最后 48 bits 得到下一跳 ip 和 port，然后转发即可。

